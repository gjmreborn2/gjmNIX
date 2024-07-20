#include <gjmnix/config.h>

const int COLUMNS = 80;
const int LINES = 24;
const int ATTRIBUTE = 7;
#define VIDEO  0xB8000

static int xpos;
static int ypos;
static volatile unsigned char *video;

void kinit_temp(unsigned long magic, unsigned long addr);
static void cls();
static void itoa(char *buf, int base, int d);
static void putchar(int c);
void kprintf(const char *format, ...);

void kinit_temp(unsigned long magic, unsigned long addr) {
	cls();
	if(magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
		kprintf("Invalid magic number: 0x%x\n", (unsigned) magic);
		return;
	}
	if(addr & 7) {
		kprintf("Unaligned mbi: 0x%x\n", addr);
		return;
	}

	parse_boot_info(addr);

	while(1);
}
