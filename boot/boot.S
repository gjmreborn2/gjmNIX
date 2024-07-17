.set ALIGN,     1<<0
.set MEMINFO,   1<<1
.set FLAGS,     ALIGN | MEMINFO
.set MAGIC,     0x1BADB002
.set CHECKSUM,  -(MAGIC + FLAGS)

# multiboot header for GRUB
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# allocate 16KB stack
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    /*
        GRUB has loaded us. State of the machine:
            32-bit protected mode
            interrupts disabled
            memory paging disabled
            floating point instructions not initialized
            instruction set extensions not initialized
            no GDT
    */
# warning: setup.s in repo is compiled using AS86!!


# set up a stack (for C etc.)
mov $stack_top, %esp

call init

cli
1:  hlt
    jmp 1b

.size _start, . - _start
