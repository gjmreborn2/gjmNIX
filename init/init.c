#include <gjmnix/config.h>
#include <gjmnix/drivers/floppy.h>
#include <gjmnix/drivers/console.h>
#include <klib/kprintf.h>

#include <stdint.h>
#include <stddef.h>

void display_boot_info(struct boot_info *binfo);

void kmain(uint32_t magic, uint32_t boot_info_addr) {
    struct boot_info binfo;

	kill_floppy_motor();

    console_init();
    console_cls();
	kprintf("Loading...\n");
    if(validate_boot_info(magic) == ERROR) {
        kprintf("Invalid magic number: 0x%x\n", magic);
        return;
    }
    if(parse_boot_info(&binfo, boot_info_addr) == ERROR) {
        kprintf("Unaligned boot info: 0x%x\n", boot_info_addr);
        return;
    }

    display_boot_info(&binfo);

    while(1) {}
}

void display_boot_info(struct boot_info *binfo) {
    kprintf("Command line = %s\n", binfo->cmd_line);
    kprintf("Boot loader name = %s\n", binfo->loader_name);
    kprintf("mem_lower = %uKB, mem_upper = %uKB\n", binfo->mem_lower, binfo->mem_upper);
    kprintf("Boot device 0x%x,%u,%u\n", binfo->boot_device_biosdev, binfo->boot_device_slice, binfo->boot_device_part);

    kprintf("mmap\n");
    for(size_t i = 0; i < 16; i++) {
        if(binfo->mmap[i].is_active == false) {
            break;
        }

        kprintf("  base_addr = 0x%x%x, length = 0x%x%x, type = 0x%x\n", binfo->mmap[i].base_addr_part1, binfo->mmap[i].base_addr_part2, binfo->mmap[i].len_part1, binfo->mmap[i].len_part2, binfo->mmap[i].type);
    }
}
