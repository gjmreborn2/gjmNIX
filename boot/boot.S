#include <gjmnix/asm_config.h>

#define GRUB_MULTIBOOT_ARCHITECTURE_I386 0

.align  8
multiboot_header:
        .long   MULTIBOOT2_HEADER_MAGIC
        .long   GRUB_MULTIBOOT_ARCHITECTURE_I386
        .long   multiboot_header_end - multiboot_header
        /*  checksum */
        .long   -(MULTIBOOT2_HEADER_MAGIC + GRUB_MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header))
multiboot_header_tag_end:
        .short MULTIBOOT_HEADER_TAG_END
        .short 0
        .long 8
multiboot_header_end:

/* allocate 16KB stack */
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
	        A20 enabled
            floating point instructions not initialized
            instruction set extensions not initialized
            no GDT
    */

/* I have to define the ROOT_DEV (where is the rootfs MINIX 1.0 file system stored) and SWAP_DEV */

/*
	This is overwritten by build.py user-space program.
	Device number is built from major + minor device subnumbers.
	Algorithm for calculating the device number:
		device number = major device nr * 256 + minor device nr
		or
		device number = (major << 8) + minor
	Major numbers:
		1 - memory
		2 - floppy disk
		3 - hard drive
		4 - ttyx
		5 - tty
		6 - parallel port
		7 - unnamed pipe

	For example device number 0x306 specifies that the root fs device is the first partition of the second hard disk.
	For next example if root fs is on the first partition of the first hard disk, the device number is 0x0301 (0x01, 0x03)

	0x300 - /dev/hd0 - entire 1st hard drive
	0x301 - /dev/hd1 - 1st partition of the first disk
*/
ROOT_DEV = 0
SWAP_DEV = 0

/*SYSTEM_SEGMENT_START = DEFAULT_SYSSEG
SYSTEM_SEGMENT_END   = SYSTEM_SEGMENT_START + DEFAULT_SYSSIZE*/

# setup.S
#	move system module (from 0x10000) to the beginning of the memory 0x0000
#	identify VGA card
#	enter protected mode, jump to head.S (32-bit mode first assembly file)

# head.S
# 	setup proper IDT, GDT, LDT
#	initialize FPU, paging
#	go to kinit

cli

# set up a stack (for C etc.)
mov $stack_top, %esp

call _init  # call the global constructors
push %ebx    # multiboot2 structure
push %eax    # magic value
call kmain

cli
1:  hlt
    jmp 1b


swap_dev:
	.word SWAP_DEV
root_dev:
	.word ROOT_DEV

.size _start, . - _start
