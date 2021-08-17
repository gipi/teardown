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
#define c0_status    $12, 0
#define c0_ebase     $15, 1
#define c0_epc       $14, 0
#define c0_cause     $13, 0
#define c0_intctl    $12, 1

#define STATUS_BEV             22
#define INTCTL_VS               5
#define INTCTL_MASK_VS 0xfffffc1f

/* 
 * dump the registers into the stack.
 *
 * ------------ higher addresses  <-- sp point here when the block starts
 *  epc
 *  state
 *  cause
 *  lo
 *  hi
 *  r31
 *  ...
 *  r0
 * ------------ lower addresses <--- sp point here when the block ends
 *
 */
.macro dump_regs
    .set push
    .set noat

    sw $k1, -0x1c($sp) /* save k1 first (at offset 0x78) */

    move $k1, $sp /* k1 should be not touched from main code */

    subu $sp, 0x94 /* FIXME: create define with sizeof */

    sw $a0, 0($sp)
    sw $a1, 4($sp)
    sw $a2, 8($sp)
    sw $a3, 12($sp)

    sw $t0, 0x10($sp)
    sw $t1, 0x14($sp)
    sw $t2, 0x18($sp)
    sw $t3, 0x1c($sp)
    sw $t4, 0x20($sp)
    sw $t5, 0x24($sp)
    sw $t6, 0x28($sp)
    sw $t7, 0x2c($sp)
    sw $t8, 0x30($sp)
    sw $t9, 0x34($sp)

    sw $s0, 0x38($sp)
    sw $s1, 0x3c($sp)
    sw $s2, 0x40($sp)
    sw $s3, 0x44($sp)
    sw $s4, 0x48($sp)
    sw $s5, 0x4c($sp)
    sw $s6, 0x50($sp)
    sw $s7, 0x54($sp)

    sw $fp, 0x58($sp)
    sw $ra, 0x5c($sp)
    sw $gp, 0x60($sp)
    sw $zero, 0x64($sp)
    sw $v0, 0x68($sp)
    sw $v1, 0x6c($sp)
    sw $at, 0x70($sp)
    sw $k0, 0x74($sp)
    /* k1 was saved at the start */
    /* now k1 containes the original stack pointer */
    sw $k1, 0x7c($sp)

    mfhi $t0
    sw $t0, 0x80($sp)
    mflo $t0
    sw $t0, 0x84($sp)

    mfc0 $t0, c0_epc
    sw $t0, 0x88($sp)
    mfc0 $t0, c0_cause
    sw $t0, 0x8c($sp)
    mfc0 $t0, c0_status
    sw $t0, 0x90($sp)
    .set pop
.endm

.macro restore_regs
    .set push
    .set noat


    lw $a0, 0($sp)
    lw $a1, 4($sp)
    lw $a2, 8($sp)
    lw $a3, 12($sp)

    lw $t0, 0x10($sp)
    lw $t1, 0x14($sp)
    lw $t2, 0x18($sp)
    lw $t3, 0x1c($sp)
    lw $t4, 0x20($sp)
    lw $t5, 0x24($sp)
    lw $t6, 0x28($sp)
    lw $t7, 0x2c($sp)
    lw $t8, 0x30($sp)
    lw $t9, 0x34($sp)

    lw $s0, 0x38($sp)
    lw $s1, 0x3c($sp)
    lw $s2, 0x40($sp)
    lw $s3, 0x44($sp)
    lw $s4, 0x48($sp)
    lw $s5, 0x4c($sp)
    lw $s6, 0x50($sp)
    lw $s7, 0x54($sp)

    lw $fp, 0x58($sp)
    lw $ra, 0x5c($sp)
    lw $gp, 0x60($sp)
    lw $zero, 0x64($sp)
    lw $v0, 0x68($sp)
    lw $v1, 0x6c($sp)
    lw $at, 0x70($sp)
    lw $k0, 0x74($sp) /* save k1 first */
    lw $k1, 0x78($sp)

    lw $t0, 0x80($sp)
    mthi $t0
    lw $t0, 0x84($sp)
    mtlo $t0

    lw $t0, 0x88($sp)
    mtc0 $t0, c0_epc
    lw $t0, 0x8c($sp)
    mtc0 $t0, c0_cause
    lw $t0, 0x90($sp)
    mtc0 $t0, c0_status

    lw $sp, 0x7c($sp) /* restore original stack pointer */

    .set pop
.endm


// setup the exception entry
.macro ex_entry
    dump_regs
    ehb // wait until exe hazard have been cleared
.endm

.macro ex_exit
    restore_regs
    ehb
    eret
.endm

