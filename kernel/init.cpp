/*
 * Provided headers in freestanding environment: stdbool.h (bool), stddef.h (size_t, NULL), stdint.h (intx_t, uintx_t, ...),
 *      float.h, iso646.h, limits.h, stdarg.h, stdalign.h, stdnoreturn.h
 */

/* extern "C" if invoked from asm - AVOID NAME MANGLING WHICH C++ DOES!!!!!! */

/*
 The GCC documentation explicitly states that libgcc requires the freestanding environment to supply the memcmp, memcpy, memmove, and memset functions, as well as abort on some platforms.*
 */

#if defined(__linux__)
#error "You are not using a cross-compiler!"
#endif

#if !defined(__i386__)
#error "gjmNIX for now only supports 32-bit mode!"
#endif

#include <multiboot_one_macros.h>
#include <multiboot_one_structs.h>

#include <gjmnix/utils.h>

/* Check if the bit BIT in FLAGS is set */
#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

#define COLUMNS 80
#define LINES 24
#define ATTRIBUTE 7
#define VIDEO 0xB8000

internal int xpos;
internal int ypos;
internal volatile unsigned char *video;

internal void cls();
internal void itoa(char *buf, int base, int d);
internal void putchar(int c);
void printf(const char *fmt, ...);

extern "C" void kernel_init(unsigned long magic, unsigned long addr) {
    // check if MAGIC is valid
    // print the multiboot information structure pointed by ADDR

    cls();
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("Invalid magic number: 0x%x\n", (unsigned) magic);
        return;
    }

    multiboot_info *mbi = (multiboot_info *) addr;
    printf("flags = 0x%x\n", (unsigned) mbi->flags);
    // are mem_* valid?
    if(CHECK_FLAG(mbi->flags, 0)) {
        printf("mem_lower = %uKB, mem_upper = %uKB\n",
            (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);
    }

    // is boot_device valid?
    if(CHECK_FLAG(mbi->flags, 1)) {
        printf("boot_device = 0x%x\n", (unsigned) mbi->boot_device);
    }

    // is the command line passed?
    if(CHECK_FLAG(mbi->flags, 2)) {
        printf("cmdline = %s\n", (char *) mbi->cmdline);
    }

    // are mods_* valid?
    if(CHECK_FLAG(mbi->flags, 3)) {
        printf("mods_count = %d, mods_addr = 0x%x\n",
            (int) mbi->mods_count, (int) mbi->mods_addr);
        
        multiboot_mod_list *mod = (multiboot_mod_list *) mbi->mods_addr;
        for(multiboot_uint32_t i = 0; i < mbi->mods_count; i++) {
            printf("    mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
                (unsigned) mod->mod_start,
                (unsigned) mod->mod_end,
                (char *) mod->cmdline);
            mod++;
        }
    }

    // bits 4 and 5 are mutually exclusive!
    if(CHECK_FLAG(mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5)) {
        printf("[ERROR] Both bits 4 and 5 are set!!! (they are mutually exclusive)\n");
        return;
    }
    // is the symbol table of a.out valid?
    if(CHECK_FLAG(mbi->flags, 4)) {
        multiboot_aout_symbol_table *multiboot_aout_sym = &(mbi->u.aout_sym);
        printf("multiboot_aout_symbol_table: tabsize = 0x%0x, "
            "strsize = 0x%x, addr = 0x%x\n",
            (unsigned) multiboot_aout_sym->tabsize,
            (unsigned) multiboot_aout_sym->strsize,
            (unsigned) multiboot_aout_sym->addr);
    }
    // is the section header table of ELF valid?
    if(CHECK_FLAG(mbi->flags, 5)) {
        multiboot_elf_section_header_table *multiboot_elf_sec = &(mbi->u.elf_sec);
        printf("multiboot_elf_sec: num = %u, size = 0x%x,"
            " addr = 0x%x, shndx = 0x%x\n",
            (unsigned) multiboot_elf_sec->num,
            (unsigned) multiboot_elf_sec->size,
            (unsigned) multiboot_elf_sec->addr,
            (unsigned) multiboot_elf_sec->shndx);
    }

    // are mmap_* valid?
    if(CHECK_FLAG(mbi->flags, 6)) {
        printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
            (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
            
            multiboot_mmap_entry *mmap = (multiboot_mmap_entry *) mbi->mmap_addr;
            while((unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length) {
                printf("    size = 0x%x, base_addr = 0x%x%08x,"
                    " length = 0x%x%08x, type = 0x%x\n",
                    (unsigned) mmap->size,
                    (unsigned) (mmap->addr >> 32),
                    (unsigned) (mmap->addr & 0xFFFFFFFF),
                    (unsigned) (mmap->len >> 32),
                    (unsigned) (mmap->len & 0xFFFFFFFF),
                    (unsigned) mmap->type);

                mmap = (multiboot_mmap_entry *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size));
            }
    }

    for(;;)
        ;
}

internal void cls() {
    video = (unsigned char *) VIDEO;
    for(int i = 0; i < COLUMNS * LINES * 2; i++) {
        *(video + i) = 0;
    }
    xpos = 0;
    ypos = 0;
}

internal void itoa(char *buf, int base, int d) {
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;

    if(base == 'd' && d < 0) {
        *p++ = '-';
        buf++;
        ud = -d;
    } else if(base == 'x') {
        divisor = 16;
    }

    do {
        int remainder = ud % divisor;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while(ud /= divisor);

    *p = 0;

    // reverse buf
    p1 = buf;
    p2 = p - 1;
    while(p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

internal void putchar(int c) {
    if(c == '\n' || c == '\r') {
    newline:
        xpos = 0;
        ypos++;
        if(ypos >= LINES) {
            ypos = 0;
        }
        return;
    }

    *(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
    *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

    xpos++;
    if(xpos >= COLUMNS) {
        goto newline;
    }
}

void printf(const char *fmt, ...) {
    char **arg = (char **) &fmt;
    int c;
    char buf[20];

    arg++;
    while((c = *fmt++) != 0) {
        if(c != '%') {
            putchar(c);
        } else {
            char *p, *p2;
            int pad0 = 0, pad = 0;

            c = *fmt++;
            if(c == '0') {
                pad0 = 1;
                c = *fmt++;
            }

            if(c >= '0' && c <= '9') {
                pad = c - '0';
                c = *fmt++;
            }

            switch(c) {
                case 'd':
                case 'u':
                case 'x':
                    itoa(buf, c, *((int *) arg++));
                    p = buf;
                    goto string;
                    break;
                case 's':
                    p = *arg++;
                    if(!p) {
                        p = "(null)";
                    }

                string:
                    for(p2 = p; *p2;p2++)
                        ;
                    for(;p2 < p + pad; p2++) {
                        putchar(pad0 ? '0' : ' ');
                    }
                    while(*p) {
                        putchar(*p++);
                    }
                    break;
                default:
                    putchar(*((int *) arg++));
                    break;
            }
        }
    }
}