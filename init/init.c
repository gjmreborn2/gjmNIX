#include <gjmnix/config.h>
#include <gjmnix/drivers/floppy.h>
#include <gjmnix/drivers/console.h>
#include <klib/kprintf.h>

#include <stdint.h>
#include <stddef.h>

void display_boot_info(struct boot_info *binfo);

/* from linker script */
extern uint32_t kernel_start;
extern uint32_t kernel_end;

extern void reprogram_pic();

void kmain(uint16_t *root_dev, uint16_t *swap_dev, uint32_t magic, uint32_t boot_info_addr) {
    struct boot_info binfo;

	kill_floppy_motor();
    reprogram_pic();

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

    /*
        add current cursor position, hdd data (cylinders, heads, start cylinder, pre-compensation cylinder, maximum ECC burst size, control byte, standard timeout value, format timeout value, detect drive timeout value, head landing (stop) cylinder number, number of sectors per track)
        , nr of screen columns, display mode, current display page
    */
    display_boot_info(&binfo);
    uint16_t ROOT_DEV = *root_dev;
    uint16_t SWAP_DEV = *swap_dev;
    kprintf("root dev = 0x%x, swap dev = 0x%x\n", ROOT_DEV, SWAP_DEV);
    kprintf("kernel base address: = 0x%x, kernel end address = 0x%x\n", &kernel_start, &kernel_end);
    kprintf("kernel size: %d bytes\n", &kernel_end - &kernel_start);

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
