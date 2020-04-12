ENTRY(_start)

MEMORY
{
    adfu (rx) : ORIGIN = 0xa0010000, LENGTH = 0x280
}

SECTIONS
{
    .text :
    {
        *(.text*)
    } > adfu
}

