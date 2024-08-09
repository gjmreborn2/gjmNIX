#include <klib/kutils.h>

#include <stdint.h>
#include <string.h>

#include <gjmnix/asm_config.h>
#include <gjmnix/config.h>

struct multiboot_header {
	/* Must be MULTIBOOT_MAGIC */
	uint32_t magic;

	/* ISA */
	uint32_t architecture;

	/* Total header length */
	uint32_t header_length;

	/* The above fields + this one must equal 0 % 2^32 */
	uint32_t checksum;
};

struct multiboot_header_tag {
	uint16_t type;
	uint16_t flags;
	uint32_t size;
};

struct multiboot_header_tag_information_request {
	uint16_t type;
	uint16_t flags;
	uint32_t size;
	uint32_t requests[0];
};

struct multiboot_header_tag_address {
	uint16_t type;
	uint16_t flags;
	uint32_t size;
	uint32_t header_addr;
	uint32_t load_end_addr;
	uint32_t bss_end_addr;
};

struct multiboot_header_tag_entry_address {
  uint16_t type;
  uint16_t flags;
  uint32_t size;
  uint32_t entry_addr;
};

struct multiboot_header_tag_console_flags {
  uint16_t type;
  uint16_t flags;
  uint32_t size;
  uint32_t console_flags;
};

struct multiboot_header_tag_framebuffer {
  uint16_t type;
  uint16_t flags;
  uint32_t size;
  uint32_t width;
  uint32_t height;
  uint32_t depth;
};

struct multiboot_header_tag_module_align {
  uint16_t type;
  uint16_t flags;
  uint32_t size;
};

struct multiboot_header_tag_relocatable {
  uint16_t type;
  uint16_t flags;
  uint32_t size;
  uint32_t min_addr;
  uint32_t max_addr;
  uint32_t align;
  uint32_t preference;
};

struct multiboot_color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

struct multiboot_mmap_entry {
  uint64_t addr;
  uint64_t len;
#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5
  uint32_t type;
  uint32_t zero;
};
typedef struct multiboot_mmap_entry multiboot_memory_map_t;

struct multiboot_tag {
  uint32_t type;
  uint32_t size;
};

struct multiboot_tag_string {
  uint32_t type;
  uint32_t size;
  char string[0];
};

struct multiboot_tag_module {
  uint32_t type;
  uint32_t size;
  uint32_t mod_start;
  uint32_t mod_end;
  char cmdline[0];
};

struct multiboot_tag_basic_meminfo {
  uint32_t type;
  uint32_t size;
  uint32_t mem_lower;
  uint32_t mem_upper;
};

struct multiboot_tag_bootdev {
  uint32_t type;
  uint32_t size;
  uint32_t biosdev;
  uint32_t slice;
  uint32_t part;
};

struct multiboot_tag_mmap {
  uint32_t type;
  uint32_t size;
  uint32_t entry_size;
  uint32_t entry_version;
  struct multiboot_mmap_entry entries[0];
};

struct multiboot_vbe_info_block {
  uint8_t external_specification[512];
};

struct multiboot_vbe_mode_info_block {
  uint8_t external_specification[256];
};

struct multiboot_tag_vbe {
  uint32_t type;
  uint32_t size;

  uint16_t vbe_mode;
  uint16_t vbe_interface_seg;
  uint16_t vbe_interface_off;
  uint16_t vbe_interface_len;

  struct multiboot_vbe_info_block vbe_control_info;
  struct multiboot_vbe_mode_info_block vbe_mode_info;
};

struct multiboot_tag_framebuffer_common {
  uint32_t type;
  uint32_t size;

  uint64_t framebuffer_addr;
  uint32_t framebuffer_pitch;
  uint32_t framebuffer_width;
  uint32_t framebuffer_height;
  uint8_t framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED      0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB          1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT     2
  uint8_t framebuffer_type;
  uint16_t reserved;
};

struct multiboot_tag_framebuffer {
  struct multiboot_tag_framebuffer_common common;

  union {
    struct {
      uint16_t framebuffer_palette_num_colors;
      struct multiboot_color framebuffer_palette[0];
    };

    struct {
      uint8_t framebuffer_red_field_position;
      uint8_t framebuffer_red_mask_size;
      uint8_t framebuffer_green_field_position;
      uint8_t framebuffer_green_mask_size;
      uint8_t framebuffer_blue_field_position;
      uint8_t framebuffer_blue_mask_size;
    };
  };
};

struct multiboot_tag_elf_sections {
  uint32_t type;
  uint32_t size;
  uint32_t num;
  uint32_t entsize;
  uint32_t shndx;
  char sections[0];
};

struct multiboot_tag_apm {
  uint32_t type;
  uint32_t size;
  uint16_t version;
  uint16_t cseg;
  uint32_t offset;
  uint16_t cseg_16;
  uint16_t dseg;
  uint16_t flags;
  uint16_t cseg_len;
  uint16_t cseg_16_len;
  uint16_t dseg_len;
};

struct multiboot_tag_efi32 {
  uint32_t type;
  uint32_t size;
  uint32_t pointer;
};

struct multiboot_tag_efi64 {
  uint32_t type;
  uint32_t size;
  uint64_t pointer;
};

struct multiboot_tag_smbios {
  uint32_t type;
  uint32_t size;
  uint8_t major;
  uint8_t minor;
  uint8_t reserved[6];
  uint8_t tables[0];
};

struct multiboot_tag_old_acpi {
  uint32_t type;
  uint32_t size;
  uint8_t rsdp[0];
};

struct multiboot_tag_new_acpi {
  uint32_t type;
  uint32_t size;
  uint8_t rsdp[0];
};

struct multiboot_tag_network {
  uint32_t type;
  uint32_t size;
  uint8_t dhcpack[0];
};

struct multiboot_tag_efi_mmap {
  uint32_t type;
  uint32_t size;
  uint32_t descr_size;
  uint32_t descr_vers;
  uint8_t efi_mmap[0];
};

struct multiboot_tag_efi32_ih {
  uint32_t type;
  uint32_t size;
  uint32_t pointer;
};

struct multiboot_tag_efi64_ih {
  uint32_t type;
  uint32_t size;
  uint64_t pointer;
};

struct multiboot_tag_load_base_addr {
  uint32_t type;
  uint32_t size;
  uint32_t load_base_addr;
};

int validate_boot_info(uint32_t boot_magic) {
    if(boot_magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        return ERROR;
    }
    return SUCCESS;
}

static void parse_mmap(struct boot_info *binfo, struct multiboot_tag *mmap_tag) {
    for(size_t i = 0; i < 16; i++) {
        binfo->mmap[i].is_active = false;
    }

    multiboot_memory_map_t *mmap;
    size_t mmap_counter = 0;

    for(mmap = ((struct multiboot_tag_mmap *) mmap_tag)->entries; (uint8_t *) mmap < (uint8_t *) mmap_tag + mmap_tag->size; mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + ((struct multiboot_tag_mmap *) mmap_tag)->entry_size)) {
        binfo->mmap[mmap_counter].is_active = true;

        binfo->mmap[mmap_counter].base_addr_part1 = mmap->addr >> 32;
        binfo->mmap[mmap_counter].base_addr_part2 = mmap->addr & 0xFFFFFFFF;
        binfo->mmap[mmap_counter].len_part1 = mmap->len >> 32;
        binfo->mmap[mmap_counter].len_part2 = mmap->len & 0xFFFFFFFF;
        binfo->mmap[mmap_counter].type = mmap->type;

        mmap_counter++;
    }
}

int parse_boot_info(struct boot_info *binfo, uint32_t multiboot2_address) {
    if(multiboot2_address & 7) {
        return ERROR;
    }
    if(!binfo) {
        return ERROR;
    }

    struct multiboot_tag *tag;
/*    uint32_t size = *(uint32_t *) multiboot2_address;*/
    for(tag = (struct multiboot_tag *) (multiboot2_address + 8); tag->type != MULTIBOOT_TAG_TYPE_END; tag = (struct multiboot_tag *) ((uint8_t *) tag + ((tag->size + 7) & ~7))) {
        uint32_t tag_type = tag->type;
/*        uint32_t tag_size = tag->size;*/
        if(tag_type == MULTIBOOT_TAG_TYPE_CMDLINE) {
            strcpy(binfo->cmd_line, ((struct multiboot_tag_string *) tag)->string);
        } else if(tag_type == MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME) {
            strcpy(binfo->loader_name, ((struct multiboot_tag_string *) tag)->string);
        } else if(tag_type == MULTIBOOT_TAG_TYPE_BASIC_MEMINFO) {
            binfo->mem_lower = ((struct multiboot_tag_basic_meminfo *) tag)->mem_lower;
            binfo->mem_upper = ((struct multiboot_tag_basic_meminfo *) tag)->mem_upper;
        } else if(tag_type == MULTIBOOT_TAG_TYPE_BOOTDEV) {
            binfo->boot_device_biosdev = ((struct multiboot_tag_bootdev *) tag)->biosdev;
            binfo->boot_device_slice = ((struct multiboot_tag_bootdev *) tag)->slice;
            binfo->boot_device_part = ((struct multiboot_tag_bootdev *) tag)->part;
        } else if(tag_type == MULTIBOOT_TAG_TYPE_MMAP) {
            parse_mmap(binfo, tag);
        }
    }

    return SUCCESS;
}

