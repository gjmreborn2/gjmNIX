;.set ALIGN,     1<<0
;.set MEMINFO,   1<<1
;.set FLAGS,     ALIGN | MEMINFO
;.set MAGIC,     0x1BADB002
;.set CHECKSUM,  -(MAGIC + FLAGS)
;
;# multiboot header for GRUB
;.section .multiboot
;.align 4
;.long MAGIC
;.long FLAGSGRUB_MULTIBOOT_ARCHITECTURE_I386
;.long CHECKSUM

#include <gjmnix/config.h>

#define STACK_SIZE 0x4000       /* ??? */

.set FLAGS (1<<0) | (1<<1)

.text
.global start, _start
start:
_start:
    jmp multiboot_entry

.align 8
multiboot_header:
    .long MULTIBOOT2_HEADER_MAGIC
    .long FLAGS
    .long multiboot_header_end - multiboot_header
    .long -(MULTIBOOT2_HEADER_MAGIC + FLAGS + (multiboot_header_end - multiboot_header))
address_tag_start:
    .short MULTIBOOT_HEADER_TAG_ADDRESS
    .short MULTIBOOT_HEADER_TAG_OPTIONAL
    .long address_tag_end - address_tag_start
    /* entry_addr */
    .long multiboot_entry
entry_address_tag_end:

framebuffer_tag_start:
    .short MULTIBOOT_HEADER_TAG_FRAMEBUFFER
    .short MULTIBOOT_HEADER_TAG_OPTIONAL
    .long framebuffer_tag_end - framebuffer_tag_start
    .long 1024
    .long 768
    .long 32
framebuffer_tag_end:
    .short MULTIBOOT_HEADER_TAG_END
    .short 0
    .long 8
multiboot_header_end:

multiboot_entry:
    /* initialize stack pointer */
    movl $(stack + STACK_SIZE), %esp

    /* reset EFLAGS */
    pushl $0
    popf

    pushl %ebx  # address of multiboot2 info structure
    pushl %eax  # magic value
    call kinit_temp     # TODO: REPLACE

    pushl $halt_message
    call kprintf
loop:
    hlt
    jmp loop

halt_message:   .asciz "Unknown error during kernel initialization. System stalled and now is halted"

/* stack area */
.comm   stack, STACK_SIZE

;# allocate 16KB stack
;.section .bss
;.align 16
;stack_bottom:
;.skip 16384
;stack_top:
;
;.section .text
;.global _start
;.type _start, @function
;_start:
;    /*
;        GRUB has loaded us. State of the machine:
;            32-bit protected mode
;            interrupts disabled
;            memory paging disabled
;	    A20 enabled
;            floating point instructions not initialized
;            instruction set extensions not initialized
;            no GDT
;    */
;
;/* I have to define the ROOT_DEV (where is the rootfs MINIX 1.0 file system stored) and SWAP_DEV */
;
;# setup.S
;#	move system module (from 0x10000) to the beginning of the memory 0x0000
;#	identify VGA card
;#	enter protected mode, jump to head.S (32-bit mode first assembly file)
;
;# head.S
;# 	setup proper IDT, GDT, LDT
;#	initialize FPU, paging
;#	go to kinit
;
;SYSTEM_SEGMENT_START = DEFAULT_SYSSEG
;SYSTEM_SEGMENT_END   = SYSTEM_SEGMENT_START + DEFAULT_SYSSIZE
;
;/*
;	This is overwritten by build.py user-space program.
;	Device number is built from major + minor device subnumbers.
;	Algorithm for calculating the device number:
;		device number = major device nr * 256 + minor device nr
;		or
;		device number = (major << 8) + minor
;	Major numbers:
;		1 - memory
;		2 - floppy disk
;		3 - hard drive
;		4 - ttyx
;		5 - tty
;		6 - parallel port
;		7 - unnamed pipe
;
;	For example device number 0x306 specifies that the root fs device is the first partition of the second hard disk.
;	For next example if root fs is on the first partition of the first hard disk, the device number is 0x0301 (0x01, 0x03)
;
;	0x300 - /dev/hd0 - entire 1st hard drive
;	0x301 - /dev/hd1 - 1st partition of the first disk
;*/
;ROOT_DEV = 0
;SWAP_DEV = 0
;
;cli
;#call kill_floppy_motor
;
;# set up a stack (for C etc.)
;mov $stack_top, %esp
;
;call _init  # call the global constructors
;push ebx    # multiboot2 structure
;push eax    # magic value
;call kinit
;
;cli
;1:  hlt
;    jmp 1b
;
;# maybe implement it in C?
;#kill_floppy_motor:
;#	pushw dx
;#	movw #0x3F2, dx		# floppy controller port DOR
;#	movb #0, al		# A drive, close FDC, disable DMA, interrupt, turn off motor
;#	outb
;#	popw dx
;#	ret
;
;swap_dev:
;	.word SWAP_DEV
;root_dev:
;	.word ROOT_DEV
;
;.size _start, . - _start
