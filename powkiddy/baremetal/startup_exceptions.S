/*
 * Define very low-level routine and memory layout
 * in order to have entry point and exceptions/IRQs
 * defined corretly.
 */

#define c0_status    $12, 0
#define c0_ebase     $15, 1
#define STATUS_BEV   22

/*****************************************
 *
 * Entry point
 * -----------
 *
 * Setup the stack, the EBase register and
 * jump the the C code
 *
 ******************************************/
    .section ".startup.text","ax"
    .global _entry
    .type    _entry,@function;
    .ent     _entry;


.align 4
_entry:
    lui $sp, %hi(_stack_start)
    ori $sp, $sp, %lo(_stack_start)

    # set BEV to 1
    mfc0 $k0, c0_status
    or $k0, $k0, (1 << STATUS_BEV)
    mtc0 $k0, c0_status

    # NOTE: in the BROM code also the WG bit is se
    # in the EBase register but the moving of the
    # vector base seems to work nonetheless
    lui $k0, %hi(__irq_base)
    ori $k0, $k0, %lo(__irq_base)
    mtc0 $k0, c0_ebase

    # set BEV to 0
    mfc0 $k0, c0_status
    xor $k0, $k0, (1 << STATUS_BEV)
    mtc0 $k0, c0_status

    j main


    .end	_entry;
	.size	_entry, .-_entry;


/*******************************
 *
 * IRQs and exceptions handling
 * ----------------------------
 * From the BROM we have Cause_IV = 1, Status_BEV = 0
 * and IntCtl_VS = 0x8: this causes the interrupts
 * to be vectored with 0x100 bytes offset (plus an implied
 * starting offset of 0x200 from what inside EBase).
 *
 ******************************/
    .section ".eirq.text","ax"
    .type    __irq_base,@function
    .ent     __irq_base

.align 12 /* this MUST be 4k aligned */
.global __irq_base
__irq_base:
    li $a0, 'A'
    lui $t0, %hi(printByte)
    ori $t0, $t0, %lo(printByte)
    jr $t0

    .end __irq_base;
    .size __irq_base, .-__irq_base;

.align 8 /* 0x100 */
.global __cache_error_eirq
__cache_error_eirq:
    li $a0, 'A'
    lui $t0, %hi(printByte)
    ori $t0, $t0, %lo(printByte)
    jr $t0
    eret

.align 7 /* 0x180 */
.global __general_eirq
__general_eirq:
    li $a0, 'B'
    lui $t0, %hi(printByte)
    ori $t0, $t0, %lo(printByte)
    jr $t0
    eret

.align 9 /* there is a 0x200 offset */
.global __eirq0
__eirq0:
    li $a0, '0'
    lui $t0, %hi(printByte)
    ori $t0, $t0, %lo(printByte)
    jr $t0
    eret
    .byte 0xaa

.align 8 /* 0x300 */
.global __eirq1
__eirq1:
    li $a0, '1'
    lui $t0, %hi(printByte)
    ori $t0, $t0, %lo(printByte)
    jr $t0
    eret
    .byte 0xab

.align 8 /* 0x400 */
.global __eirq2
__eirq2:
    li $a0, '2'
    lui $t0, %hi(printByte)
    ori $t0, $t0, %lo(printByte)
    jr $t0
    eret
    .byte 0xac

.align 8 /* 0x500 */
.global __eirq3
__eirq3:
    li $a0, '3'
    lui $t0, %hi(printByte)
    ori $t0, $t0, %lo(printByte)
    jr $t0
    eret
    .byte 0xad

.align 8 /* 0x600 */
.global __eirq4
__eirq4:
    li $a0, '4'
    lui $t0, %hi(printByte)
    ori $t0, $t0, %lo(printByte)
    jr $t0
    eret
    .byte 0xad

.align 8 /* 0x700 */
.global __eirq5
__eirq5:
    li $a0, '5'
    lui $t0, %hi(printByte)
    ori $t0, $t0, %lo(printByte)
    jr $t0
    eret
    .byte 0xad

.align 8 /* 0x800 */
.global __eirq6
__eirq6:
    li $a0, '6'
    lui $t0, %hi(printByte)
    ori $t0, $t0, %lo(printByte)
    jr $t0
    eret
    .byte 0xad
