ENTRY(_start)

SECTIONS
{
#    . = 16M;
    . = 0x0;
    kernel_start = .;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    /* read only data */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /* read-write data (data initialized) */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /* read-write data and stack (data uninitialized) */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    kernel_end = .;
}