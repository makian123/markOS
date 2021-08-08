ENTRY(kmain)

SECTIONS {
    . = 1M;

    .stivale2hdr ALIGN(4K) : {
        KEEP(*(.stivale2hdr))
    }

    .boot : ALIGN(4K)
    {
        *(.multiboot_header)
    }

    .text : ALIGN(4K)
    {
        *(.text)
    }

    .rodata : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(.COMMON)
        *(.bss)
    }
}