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
#include "hal.h"
#include <uart.h>
#include "utils.h"
#include "log.h"
#include "regs_io.h"
#include "regs.h"
#include "brom.h"
#include "usb.h"

#define VERSION "0.1"

const char* BANNER = "\n\nOPEN ADEC "VERSION" for "BOARD"\n";


int main() {
    hal_hardware_setup();
    serial_init();
    log(BANNER);

    map_setup();
    usb_setup();

    _delay(10000);

    char looper[] = "|\\-/";

    u32 counter = 0;

    while (1) {
        serial_putc(looper[counter++ % 4]);
        serial_putc('\r');
        _delay(10000);
    }
}
