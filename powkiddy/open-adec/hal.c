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
#include "regs.h"
#include "uart.h"
#include "utils.h"
/*
 * Pretty much copy-pasted from the reversing of adec09_1.bin.
 *
 * I have no idea of what this stuff is doing if not reading
 * datasheets of similar chips and believing that the registers
 * are the same.
 */
static void setup() {
    // this line below skews the clock,
    // the original adec's code sets this register
    // after having checked some values at bfc00012
    //*PMU_UNKNOWN = (*PMU_UNKNOWN & 0xffffff7f) | 0x80;
    *PMU_VDD_CTL = 0xdaaa00;

    *CMU_SDRCLK = 0x1;

    _delay(24000);

    *CMU_COREPLL = 0x2328;

    _delay(60000);

    *CMU_SDRCLK = 0x42;

    _delay(60000);

    *CMU_DEVCLKEN = 0xe4123f5;

    _delay(60000);

    *PMU_CTL1 |= 0x400000;
}

void hal_hardware_setup() {
    setup();
    serial_init();
}
