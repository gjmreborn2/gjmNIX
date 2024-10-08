#pragma once

#include <stdint.h>
#include <stdbool.h>

// error handling
#define ERROR   false
#define SUCCESS true

// for uname()
#define UNAME_SYSNAME 	"gjmNIX"
#define UNAME_NODENAME  "(none)"	// this should be set by sethostname()
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

 struct boot_mem_map {
    bool is_active;         /* if false then the current record is not active, is empty */
    /*0xPART1PART2*/
    uint32_t base_addr_part1;
    uint32_t base_addr_part2;
    uint32_t len_part1;
    uint32_t len_part2;
    uint32_t type;
 };

struct boot_info {
    char cmd_line[32];
    char loader_name[32];
    uint32_t mem_lower;     /* in KBs */
    uint32_t mem_upper;
    uint32_t boot_device_biosdev;
    uint32_t boot_device_slice;
    uint32_t boot_device_part;
    struct boot_mem_map mmap[16];
};

int validate_boot_info(uint32_t boot_magic);
int parse_boot_info(struct boot_info *binfo, uint32_t multiboot2_address);
