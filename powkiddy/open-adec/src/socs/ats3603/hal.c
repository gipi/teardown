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
static void hw_register_setup() {
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

void rtc_configuration() {
          u32 rtc_value = r32(RTC_WDCTL);
          rtc_value &= 0xffffffef;
          w32(RTC_WDCTL, rtc_value | 1);
}

struct uart_t uart;


/*
 * This appears to be necessary from the datasheet but the serial
 * works anyway if this it's not invoked.
 */
static void enable_uart() {
    *CMU_UART1CLK |= (1 << U1EN); /* this seems to be missing from the brec code */
}


static reg get_uart_clk() {
    return CMU_UART1CLK;
}

static void set_baudrate(uint32_t baudrate, uint32_t clock) {
    u32* uartclk = get_uart_clk();
    w32(uartclk,(uint16_t)((clock * 1000000UL)/(baudrate * 8UL)) - 1);
}

/*
 * UART configuration.
 *
 * Some registers are unknown, simply copied from reversing
 * the powkiddy brec code.
 */
void uart_init(int baudrate) {
    *CMU_DEVRST = *CMU_DEVRST | 0x10;
    *CMU_FOO = (*CMU_FOO & 0xffffffef) | 0x10;
    *GPIO_MFCTL1 = (*GPIO_MFCTL1 & 0xffff9fff) | 0x80002000;
    *SERIAL_CTL = (1 << 15) | (1 << 1) | (1 << 0); /* enable UART1, 8bit, 1 stop */

    set_baudrate(baudrate, 24);
    //enable_uart();
}

static void uart_setup() {
    uart.base = (struct uart_reg_t*)UART_BASE;
    uart.init = uart_init;
}

void hal_hardware_setup() {
    hw_register_setup();
    rtc_configuration();
    uart_setup();
}
