#pragma once

#include <stdint.h>

// for uname()
#define UNAME_SYSNAME 	"gjmNIX"
#define UNAME_NODENAME "(none)"	// this should be set by sethostname()
#define UNAME_RELEASE	 "0"
#define UNAME_VERSION	 "0.1"
#define UNAME_MACHINE	 "i386"		// 32-bit x86 architecture

/*
 * load system at 0x10000
 */
#define DEFAULT_SYSSEG		0x1000
/* number of 16-bytes units to be loaded (system module - boot.S and whole kernel)
 * for example 0x3000 is 0x30000 bytes that are equal to 196 KB.
 */
#define DEFAULT_SYSSIZE		0x3000

/*
 * The bootsect.S tries to get drive parameters from BIOS but if this will fail, the OS will hang.
 *
 * Here is the place for defining the harddisk drive parameters by HDD_TYPE macro which must be in the following format:
 * { head, sector, cylinder, wpcom, lzone, ctl}
 * for example:
 *
 * #define HDD_TYPE	{4, 17, 615, 300, 615, 8}
 *
 * NOTE: ctl must be equal to 0 for all HDD's with heads <= 8, and ctl=8 for drives with more than 8 heads
 */

#define MULTIBOOT2_HEADER_MAGIC                 0xE85250D6
#define MULTIBOOT2_BOOTLOADER_MAGIC	            0x36D76289
#define MULTIBOOT_HEADER_TAG_ADDRESS  		        2
#define MULTIBOOT_HEADER_TAG_OPTIONAL 		        1
#define MULTIBOOT_HEADER_TAG_FRAMEBUFFER  	     5

struct boot_info {

};

extern struct boot_info boot_info;

//extern struct boot_info parse_boot_info(uint32_t multiboot2_address);
extern void parse_boot_info(unsigned long addr);

