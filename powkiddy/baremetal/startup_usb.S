# This is used only to setup the stack pointer (for now)
# Take in mind that the code is run after adec in ADFU mode
.global _start
_start:
    lui $sp, 0xb406
    addiu $sp, $sp, 0x0400

    j main
