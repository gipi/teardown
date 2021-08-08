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

void usb_setup();
void usb_handle_ep1out();

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

struct usb_cbw_packet {
    u32 signature;
    u32 tag;
    u32 transferLength;
    u8  flags;
    u8  LUN;
    u8  cmdLength;
    u8  cmd;
    u32 arg0;
    u32 arg1;
    u32 arg2;
    u32 padding;
};

struct usb_csw_packet {
    u32 signature;
    u32 tag;
    u32 residue;
    u8  status;
};
#endif
