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
    int counter;
    u32 sdr_stat;
    u32 cmu_devrst;

    w32(CMU_BUSCLK,0x2000034c);

    counter = 200;
    do {
        counter = counter + -1;
    } while (counter != 0);
    // _DAT_b0038008 = 0;

    w32(CMU_COREPLL,0x388d94b);
    w32(CMU_BUSCLK,0x2000038c);
    w32(CMU_DEVCLKEN,0xffffffff);
    w32(CMU_DDRPLL,0x2c333fc);

    counter = 100000;
    do {
        counter = counter + -1;
    } while (counter != 0);

    cmu_devrst = r32(CMU_DEVRST);
    w32(CMU_DEVRST, cmu_devrst & 0xfffffffe);
    w32(CMU_DEVRST, (cmu_devrst & 0xfffffffe) | 1);

    w32(SDR_CTL,0x30166);
    w32(SDR_PADCTRL1,0x1fe0066);
    w32(SDR_PADCTRL2,0x24152415);
    w32(SDR_PADCTRL3,0x2660266);
    w32(SDR_CLKDLY,0x20a0000);
    w32(SDR_TIMING,0x25aa7377);
    w32(SDR_MSTDLY,0x51000);
    w32(CMU_DDRPLL2,0x51);
    w32(SDR_UNKNOWN,0x1808080);
    w32(SDR_MODE,0x521);
    w32(SDR_EMODE1,4);
    w32(SDR_EMODE2,0x200);
    w32(SDR_EMODE3,0);
    w32(SDR_EN,1);
    w32(SDR_LATENCY,0x8000605);
    w32(SDR_RFSH,0xd00);
    w32(SDR_CMD,0);
    w32(SDR_CMD,0xa0);
    w32(SDR_CMD,0);

    do {
        sdr_stat = r32(SDR_STAT);
    } while ((sdr_stat & 1) == 0);

    w32(SDR_ADDRSWAP,0xc05);
    w32(SDR_WEIGHT,0x10);
    w32(SDR_CLKDLY,0x10a0000);
}

struct uart_t uart;

u32 recover_divider_maybe() {
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


static void uart_set_baudrate(int baudrate) {

    u32 corepll = r32(CMU_COREPLL);
    u32 divider = recover_divider_maybe();
    u32 _baudrate = divider * 0x10 * baudrate;
    w32(CMU_UART2CLK,((corepll >> 2 & 0x7f) * 12000000) / _baudrate | 0x10000);
}

static void uart_init(int baudrate) {
    u32 devclken = r32(CMU_DEVCLKEN);
    w32(CMU_DEVCLKEN,devclken | 0x400000);

    u32 gpio_mfctl3 = r32(GPIO_MFCTL3);
    w32(GPIO_MFCTL3, (gpio_mfctl3 & 0xff03ffff) | 0x900000);

    w32(&uart.base->ctl,0xc8403);

    uart_set_baudrate(baudrate);
}

static void uart_setup() {
    uart.base = (struct uart_reg_t*)UART_BASE;
    uart.init = uart_init;
}

void hal_hardware_setup() {
    hw_register_setup();
    uart_setup();
}
