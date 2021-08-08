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
#include "utils.h"
#include "log.h"
#include "regs_io.h"
#include "regs.h"
#include "brom.h"
#include "usb.h"

#define VERSION "0.1"

const char* BANNER = "OPEN ADEC "VERSION"\n";

void rtc_configuration() {
          u32 rtc_value = r32(RTC_WDCTL);
          rtc_value &= 0xffffffef;
          w32(RTC_WDCTL, rtc_value | 1);
}

int main() {
    hal_hardware_setup();
    log(BANNER);

    rtc_configuration();
    map_setup();
    usb_setup();

    _delay(10000);


    while (1);
}
