ENTRY(_start)

MEMORY
{
    adfu (rx) : ORIGIN = 0xb4060000, LENGTH = 0x500
}

SECTIONS
{
    .text :
    {
        *(.text*)
    } > adfu
}

