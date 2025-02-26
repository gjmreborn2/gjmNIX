#pragma once

/* How many bytes from the start of the file we search for the header */
#define MULTIBOOT_SEARCH                8192
#define MULTIBOOT_HEADER_ALIGN          4

/* Magic field */
#define MULTIBOOT_HEADER_MAGIC          0x1BADB002

/* This should be in eax */
#define MULTIBOOT_BOOTLOADER_MAGIC      0x2BADB002

/* Alignment of multiboot modules */
#define MULTIBOOT_MOD_ALIGN             0x00001000

/* Alignment of the multiboot info structure */
#define MULTIBOOT_INFO_ALIGN            0x00000004

/* [SECTION] Now flags set in the 'flags' field of the multiboot header [SECTION] */

/* Align all boot modules on i386 page (4KB) boundaries */
#define MULTIBOOT_PAGE_ALIGN            0x00000001

/* Must pass memory info to OS */
#define MULTIBOOT_MEMORY_INFO           0x00000002

/* Must pass video info to OS */
#define MULTIBOOT_VIDEO_MODE            0x00000004

/* Indicates the use of the address fields in the header */
#define MULTIBOOT_AOUT_KLUDGE           0x00010000

/* [SECTION] Now flags to be set in the 'flags' field of the multiboot info structure [SECTION] */

/* is there basic lower/upper memory info? */
#define MULTIBOOT_INFO_MEMORY           0x00000001
/* is there a boot device set? */
#define MULTIBOOT_INFO_BOOTDEV          0x00000002
/* is the command-line defined? */
#define MULTIBOOT_INFO_CMDLINE          0x00000004
/* are there modules? */
#define MULTIBOOT_INFO_MODS             0x00000008
/* is there a symbol table loaded? */
#define MULTIBOOT_INFO_AOUT_SYMS        0x00000010
/* is there an ELF section header table? */
#define MULTIBOOT_INFO_ELF_SHDR         0x00000020
/* is there a full memory map? */
#define MULTIBOOT_INFO_MEM_MAP          0x00000040
/* is there drive info? */
#define MULTIBOOT_INFO_DRIVE_INFO       0x00000080
/* is there a config table? */
#define MULTIBOOT_INFO_CONFIG_TABLE     0x00000100
/* is there a boot loader name? */
#define MULTIBOOT_INFO_BOOT_LOADER_NAME 0x00000200
/* is there a APM table? */
#define MULTIBOOT_INFO_APM_TABLE        0x00000400
/* is there video info? */
#define MULTIBOOT_INFO_VBE_INFO         0x00000800
#define MULTIBOOT_INFO_FRAMEBUFFER_INFO 0x00001000
