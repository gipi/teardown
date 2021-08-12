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
#include "uart.h"

static void set_baudrate(uint32_t baudrate, uint32_t clock) {
    *UART1_CLK = (uint16_t)((clock * 1000000UL)/(baudrate * 8UL)) - 1;
}

/*
 * This appears to be necessary from the datasheet but the serial
 * works anyway if this it's not invoked.
 */
static void enable_uart() {
    *UART1_CLK |= (1 << U1EN); /* this seems to be missing from the brec code */
}

/*
 * UART configuration.
 *
 * Some registers are unknown, simply copied from reversing
 * the powkiddy brec code.
 */
void serial_init() {
    *CMU_DEVRST = *CMU_DEVRST | 0x10;
    *CMU_FOO = (*CMU_FOO & 0xffffffef) | 0x10;
    *GPIO_MFCTL1 = (*GPIO_MFCTL1 & 0xffff9fff) | 0x80002000;
    *SERIAL_CTL = (1 << 15) | (1 << 1) | (1 << 0); /* enable UART1, 8bit, 1 stop */

    set_baudrate(115200, 24);
    //enable_uart();
}


void serial_putc(unsigned char c) {
    while (!is_serial_free());
    serial_write(c);
}

void serial_puts(const char* buffer) {
    const char* ptr = buffer;

    while (*ptr) {
        serial_putc(*ptr);
        ptr++;
    }
}
