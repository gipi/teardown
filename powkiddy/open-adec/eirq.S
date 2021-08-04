/*
 * Copyright 2021 Gianluca Pacchiella <gp@ktln2.org>
 *
 * This file is part of open-adec.
 *
 * open-adec is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * open-adec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with open-adec.  If not, see <http://www.gnu.org/licenses/>.
 */
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
#include "asm.h"
    .section ".eirq.text","ax"
    .type    __irq_base,@function
    .ent     __irq_base

.align 12 /* this MUST be 4k aligned */
.global __irq_base
__irq_base:
    li $a0, 'A'
    lui $t0, %hi(serial_putc)
    ori $t0, $t0, %lo(serial_putc)
    jr $t0
    nop
    nop
    eret

    .end __irq_base;
    .size __irq_base, .-__irq_base;

.align 8 /* 0x100 */
.global __cache_error_eirq
__cache_error_eirq:
    li $a0, 'B'
    lui $t0, %hi(serial_putc)
    ori $t0, $t0, %lo(serial_putc)
    jr $t0
    nop
    nop
    eret

.align 7 /* 0x180 */
.global __general_eirq
__general_eirq:
    ex_entry
    move $a0, $sp /* pass the exception frame pointer as argument */
    lui $t0, %hi(_handle_exception)
    ori $t0, $t0, %lo(_handle_exception)
    jr $t0
    nop
    nop
    ex_exit

/* remove this entry since the previous is overlapping this entry */
#if 0
.align 9 /* there is a 0x200 offset */
.global __eirq0
__eirq0:
    li $a0, '0'
    lui $t0, %hi(serial_putc)
    ori $t0, $t0, %lo(serial_putc)
    jr $t0
    eret
    .byte 0xaa
#endif
.align 8 /* 0x300 */
.global __eirq1
__eirq1:
    li $a0, '1'
    lui $t0, %hi(serial_putc)
    ori $t0, $t0, %lo(serial_putc)
    jr $t0
    nop
    nop
    eret
    .byte 0xab

.align 8 /* 0x400 */
.global __eirq2
__eirq2:
    li $a0, '2'
    lui $t0, %hi(serial_putc)
    ori $t0, $t0, %lo(serial_putc)
    jr $t0
    eret
    .byte 0xac

.align 8 /* 0x500 */
.global __eirq3
__eirq3:
    li $a0, '3'
    lui $t0, %hi(serial_putc)
    ori $t0, $t0, %lo(serial_putc)
    jr $t0
    eret
    .byte 0xad

.align 8 /* 0x600 */
.global __eirq4
__eirq4:
    li $a0, '4'
    lui $t0, %hi(serial_putc)
    ori $t0, $t0, %lo(serial_putc)
    jr $t0
    eret
    .byte 0xad

.align 8 /* 0x700 */
.global __eirq5
__eirq5:
    li $a0, '5'
    lui $t0, %hi(serial_putc)
    ori $t0, $t0, %lo(serial_putc)
    jr $t0
    eret
    .byte 0xad

.align 8 /* 0x800 */
.global __eirq6
__eirq6:
    lui $t0, %hi(usb_handle_irq)
    ori $t0, $t0, %lo(usb_handle_irq)
    jalr $t0
    nop
    nop
    eret /* eret must be far away from delay slot! */
