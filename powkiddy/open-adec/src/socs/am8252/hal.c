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
#include <hal.h>
#include "uart.h"
#include <regs.h>
#include <regs_io.h>




/*
 * Pretty much copied from the ADECadfus.
 */
static void hw_register_setup() {
    w32(CMU_COREPLL,0x86a90dd);
    w32(CMU_BUSCLK,0x390);
    w32(CMU_DEVCLKEN,0xfffffff);
    w32(CMU_SPCLK,8);
    w32(SDR_PD_REG,0);
    u32 dVar1 = r32(MU_REG1);
    w32(MU_REG1,dVar1 & 0xffffc0ff);
    w32(SDR_CLKDLY,0xc0000014);
    w32(SDR_CTL,0xc0150);
    w32(SDR_INITD,53000);
    w32(SDR_TIMING,0xc92814a);
    w32(SDR_RFSH,0x4fc);
    w32(SDR_MODE,0x32);
    w32(SDR_EMODE,0);
    w32(SDR_WEIGHT,0x211);
    w32(SDR_PD,80000000);
    w32(SDR_SR,40000000);
    w32(SDR_EN,1);
    w32(SDR_CMD,0);
    w32(SDR_CMD,0xa0);
    do {
        dVar1 = r32(SDR_STAT);
    } while ((dVar1 & 1) == 0);
    w32(SDR_CTL,0xc0150);
}

struct uart_t uart;

u32 recover_clock_maybe() {
    u32 busclk = r32(CMU_BUSCLK);
    switch((busclk << 1) >> 0x1d) {
        default:
            return 2;
        case 1:
            return 4;
        case 2:
            return 5;
        case 3:
            return 6;
        case 4:
            return 3;
        case 5:
            return 7;
        case 6:
            return 8;
    }
}


void uart_set_baudrate(int baudrate) {

    u32 corepll =r32(CMU_COREPLL);
    u32 divider = recover_clock_maybe();
    u32 _baudrate = divider * 0x10 * baudrate;
    w32(CMU_UART2CLK,((corepll >> 2 & 0x7f) * 12000000) / _baudrate | 0x10000);
}

static void uart_init() {
    u32 devclken = r32(CMU_DEVCLKEN);
    w32(CMU_DEVCLKEN,devclken | 0x400000);

    u32 mfctl3 = r32(GPIO_MFCTL3);
    w32(GPIO_MFCTL3, (mfctl3 & 0xffe3ffff) | 0x100000);
    mfctl3 = r32(GPIO_MFCTL3);
    w32(GPIO_MFCTL3, (mfctl3 & 0xff1fffff) | 0x800000);

    w32(&uart.base->ctl,0xc8403);

    uart_set_baudrate(115200);
}

static void uart_setup() {
    uart.base = (struct uart_reg_t*)UART_BASE;
    uart.init = uart_init;
}

void hal_hardware_setup() {
    hw_register_setup();
    uart_setup();
}

reg get_uart_clk() {
    return CMU_UART2CLK;
}
