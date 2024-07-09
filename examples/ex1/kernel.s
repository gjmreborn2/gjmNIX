# 32-bit GNU assembly kernel
# Set proper stack, GDT, IDT, clock and syscall interrupts.
# Implement two processes (tasks running on ring 3).
# After CPU initialization is finished, let's jump to process 0 and let the scheduler (clock interrupt) make the rest of work.
# Author: gjm

# This kernel was loaded by bootsector.
# Initial state of CPU: interrupts disabled, 32-bit protected mode, GDT is set, IDT is nulled, no paging

LATCH = 11930       # timer count - scheduler will be triggered by timer (clock) interrupt every 10ms

TSS0_SEL = 0x20
LDT0_SEL = 0x28
TSS1_SEL = 0x30
LDT1_SEL = 0x38

SCREEN_SEL = 0x18

.text
.global startup_32
startup_32:
# First load proper data segment selector (ds) and set up the stack.
    movl $0x10, %eax
    mov %ax, %ds
    lss init_stack, %esp
# reset IDT and GDT
    lgdt lgdt_opcode
    call setup_idt
    mov $0x10, %eax # data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    lss init_stack, %esp
# setup hardware timer - channel 0, generate interrupt every 10ms
    movb $0x36, %al
    movl $0x43, %edx
    outb %al, %dx
    movl $LATCH, %eax
    movl $0x40, %edx
    outb %al, %dx
    movb %ah, %al
    outb %al, %dx
# register timer interrupt handler in IDT at item 8
    movl $0x00080000, %eax
    movw $timer_interrupt, %ax
    movw $0x8E00, %dx
    movl $0x08, %ecx
    lea idt(,%ecx,8), %esi
    movl %eax, (%esi)
    movl %edx, 4(%esi)
# register system call handler in IDT at item 0x80
    movw $system_call, %ax
    movw $0xEF00, %dx
    movl $0x80, %ecx
    lea idt(,%ecx,8), %esi
    movl %eax, (%esi)
    movl %edx, 4(%esi)
# start process 0
    pushfl
    andl $0xFFFFBFFF, (%esp)
    popfl
    movl $TSS0_SEL, %eax
    ltr %ax
    movl $LDT0_SEL, %eax
    lldt %ax
    movl $0, current
    sti
    pushl $0x17
    pushl $init_stack
    pushfl
    pushl $0x0F
    pushl $process0_main
    iret

# ==================================================================================================================
#                               FUNCTIONS
# ==================================================================================================================

# Set temporary full IDT with default interrupt handler 256 interrupt gate descriptors.
setup_idt:
    lea ignore_interrupt_handler, %edx
    movl $0x00080000, %eax
    movw %dx, %ax
    movw $0x8E00, %dx
    lea idt, %edi
    mov $256, %ecx
loop_idt:
    movl %eax, (%edi)
    movl %edx, 4(%edi)
    addl $8, %edi
    dec %ecx
    jne loop_idt
    lidt lidt_opcode
    ret

# Display characters. Current cursor position and display char in AL
# We are in 80 x 25 text mode that can fit maximum 2000 characters.
write_char:
    push %gs
    pushl %ebx
    movl $SCREEN_SEL, %ebx		# ??	z mov na movl zmienilem
    mov %bx, %gs				# ??
    movl screen_location, %ebx	# z bx na ebx zmienilem
    shl $1, %ebx
    movb %al, %gs:(%ebx)
    shr $1, %ebx
    incl %ebx
    cmpl $2000, %ebx
    jb 1f
    movl $0, %ebx
1:
    movl %ebx, screen_location
    popl %ebx
    pop %gs
    ret

.align 2
ignore_interrupt_handler:
    push %ds
    pushl %eax
    movl $0x10, %eax
    mov %ax, %ds
    movl $67, %eax      # 67 represents 'C' letter
    call write_char
    popl %eax
    pop %ds
    iret

# scheduler base, process switching
.align 2
timer_interrupt:
    push %ds
    pushl %eax
    movl $0x10, %eax
    mov %ax, %ds
    movb $0x20, %al
    outb %al, $0x20
    movl $1, %eax
    cmpl %eax, current      # check current process and switch between process 0 and process 1
    je 1f
    movl %eax, current
    ljmp $TSS1_SEL, $0      # if current process is 0, save 1 in current and execute process 1
    jmp 2f
1:
    movl $0, current        # if current process is 1, save 0 in current and execute process 0
	ljmp $TSS0_SEL, $0
2:
	popl %eax
	pop %ds
    iret

# system call int 0x80 handler
# In this syscall 0x80 version, it handles only one display char function.
.align 2
system_call:
    push %ds
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    movl $0x10, %edx
    mov %dx, %ds
    call write_char
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    pop %ds
    iret

# ==================================================================================================================
#                               VARIABLES, DATA
# ==================================================================================================================

# current process number (0 or 1)
current: .long 0

# screen current display position
screen_location: .long 0

.align 2
lidt_opcode:
    .word 256*8-1
    .long idt
lgdt_opcode:
    .word (end_gdt - gdt) - 1
    .long gdt

.align 8
idt:    .fill 256, 8, 0

gdt:
    .quad 0x0000000000000000        # not used
    .quad 0x00C09A00000007FF        # kernel code, selector 0x08
    .quad 0x00C09200000007FF        # kernel data, selector 0x10
    .quad 0x00C0920B80000002        # display text-mode mem, selector 0x18
    .word 0x68, tss0, 0xE900, 0x0   # process 0 (TSS0), selector 0x20
    .word 0x40, ldt0, 0xE200, 0x0   # process 0 LDT0, selector 0x28
    .word 0x68, tss1, 0xE900, 0x0   # process 1 (TSS1), selector 0x30
    .word 0x40, ldt1, 0xE200, 0x0   # process 1 LDT1, selector 0x38
end_gdt:

.fill 128, 4, 0     				# initial kernel stack space
init_stack:							# stack pointer
    .long init_stack				# stack segment in kernel data segment 0x10
	.word 0x10

# ==================================================================================================================
#                               PROCESSES 0
# ==================================================================================================================

.align 8
ldt0:
	.quad 0x0000000000000000		# not used
	.quad 0x00C0FA00000003FF		# local code descriptor, selector 0x0F
	.quad 0x00C0F200000003FF		# local data descriptor, selector 0x17
tss0:
	.long 0
	.long kernel_stack0, 0x10
	.long 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0
	.long LDT0_SEL, 0x8000000

.fill 128, 4, 0
kernel_stack0:						# kernel stack for process 0

process0_main:
	movl $0x17, %eax
	movw %ax, %ds
	movb $65, %al		# 'A' letter ASCII code		movl na movb
	int $0x80			# syscall
	movl $0xFFF, %ecx	# delay based on loop
1:	loop 1b
	jmp process0_main

# ==================================================================================================================
#                               PROCESSES 1
# ==================================================================================================================

.align 8
ldt1:
	.quad 0x0000000000000000		# not used
	.quad 0x00C0FA00000003FF		# code, selector 0x0F
	.quad 0x00C0F200000003FF		# data, selector 0x17
tss1:
	.long 0
	.long kernel_stack1, 0x10
	.long 0, 0, 0, 0, 0
	.long process1_main, 0x200
	.long 0, 0, 0, 0
	.long 0x17, 0x0F, 0x17, 0x17, 0x17, 0x17
	.long LDT1_SEL, 0x8000000

.fill 128, 4, 0
kernel_stack1:						# kernel stack for process 1

process1_main:
	movb $66, %al		# 'B' letter ASCII code		movl na movb zmienilem
	int $0x80			# syscall
	movl $0xFFF, %ecx	# delay based on loop
1:	loop 1b
	jmp process1_main

.fill 128, 4, 0
user_stack1:			# user stack space for task (process) 1

# system call for displaying characters on the screen (used from ring 3 code)
# reset GDT, set IDT, clock interrupt 0x08, default interrupt handler, process code of syscall 0x80, code with data of task A, B
# task A and task B, print its own identity (name) and switch to another task every 10ms (preemptive scheduling, not ancient manual CPU releasing by process)
