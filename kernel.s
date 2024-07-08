# 32-bit GNU assembly kernel
# Set proper stack, GDT, IDT, clock and syscall interrupts.
# Implement two processes (tasks running on ring 3).
# After CPU initialization is finished, let's jump to process 0 and let the scheduler (clock interrupt) make the rest of work.
# Author: gjm

# This kernel was loaded by bootsector.
# Initial state of CPU: interrupts disabled, 32-bit protected mode, GDT is set, IDT is nulled, no paging

LATCH = 11930       # timer count - scheduler will be triggered by timer (clock) interrupt every 10ms

.text
_start:
#startup_32:
# First load proper data segment selector (ds) and set up the stack.
    movl $0x10, %eax
    mov %ax, %ds

# system call for displaying characters on the screen (used from ring 3 code)
# reset GDT, set IDT, clock interrupt 0x08, default interrupt handler, process code of syscall 0x80, code with data of task A, B
# task A and task B, print its own identity (name) and switch to another task every 10ms (preemptive scheduling, not ancient manual CPU releasing by process)
