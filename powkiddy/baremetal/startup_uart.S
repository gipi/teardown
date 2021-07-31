
.global _start
_start:
    lui $sp, 0xb406
    addiu $sp, $sp, 0x7f00

    j main
