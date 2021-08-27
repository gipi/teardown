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
/*
 * Define very low-level routine and memory layout
 * in order to have entry point and exceptions/IRQs
 * defined corretly.
 */
#include "asm.h"
#include <config.h>
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
    di
    la $sp, STACK_START_ADDRESS

#ifdef EXCEPTIONS
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

    # set the vector spacing to 0x200
    mfc0 $k0, c0_intctl
    and $k0, $k0, INTCTL_MASK_VS
    ori $k0, $k0, (0x8 << INTCTL_VS)
    mtc0 $k0, c0_intctl

    # set BEV to 0
    mfc0 $k0, c0_status
    xor $k0, $k0, (1 << STATUS_BEV)
    mtc0 $k0, c0_status
#endif
    ei

    j main


    .end	_entry;
	.size	_entry, .-_entry;

