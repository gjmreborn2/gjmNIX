/*
float.h, iso646.h, limits.h, stdalign.h, stdarg.h, stdbool.h, stddef.h, stdint.h and stdnoreturn.h
*/

#include <gjmnix/config.h>
#include <gjmnix/drivers/floppy.h>

#include <stdint.h>

void kinit(uint32_t magic_boot_value, uint32_t multiboot2_address) {
	if(magic_boot_value != 0x36D76289) {
		// kprintf("Multiboot magic value is wrong (%#h). Kernel initialization failed.\n");
		while(1);
	}

	kill_floppy_motor();

	// kprintf("Loading...\n");

	
	boot_info = parse_boot_info(multiboot2_address);

	while(1);
}
