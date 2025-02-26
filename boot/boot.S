/* multiboot ONE standard, 32-bit asm */

#include <asm.h>

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384     # 16KB stack
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    /*
        Loaded state:
            32-bit protected mode
            interrupts disabled
            paging disabled
    */

    mov $stack_top, %esp

    /*
        initialization of hardware:
            FPU
            Instruction set extensions
            GDT
            Paging
            C++ features: global constructors, exceptions
    */
    call _init      # call the global constructors

    /* Push the pointer to the Multiboot information structure. */
    pushl   %ebx
    /* Push the magic value. */
    pushl   %eax
    call kernel_init

    cli
1:
    hlt
    jmp 1b

.size _start, . - _start
