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
#ifndef __USB_H__
#define __USB_H__
#include "regs_io.h"

#define is_device2host(x)      ((x.bmRequestType) & 0x80)
#define setup_get_type(x)      (((x.bmRequestType) >> 5) & 0x3)
#define setup_get_recipient(x) ((x.bmRequestType) & 0xf)


enum bRequest_enum {
    GET_STATUS = 0x00,
    CLEAR_FEATURE = 0x01,
    SET_FEATURE = 0x03,
    SET_DESCRIPTOR = 0x06,
    SET_CONFIGURATION = 0x09
};

struct usb_setup_packet {
    u8  bmRequestType;
    u8  bRequest;
    u16 wValue;
    u16 wIndex;
    u16 wLength;
};

#endif
