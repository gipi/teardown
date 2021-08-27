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
 * NOTES
 * -----
 *
 * FIFOxDAT simply overwrites the internal buffer when receives it from outside
 *
 * Probably OUT1CS, when you write 0 into it, reset the internal index pointing
 * to what has been read last: when to the ADFU in the BROM is sent first an
 * upload command (05) (followed by the open-adec firmware itself) and the the
 * execute command (10) that is a simple CBW packet you have the following
 * reading the FIFO1DAT register:
 *
 * 55 53 42 43 00 00 00 00 00 00 00 00 00 00 00 10   <--- last cmd 10 packet
 * 00 00 06 b4 00 00 00 00 00 00 00 00 00 00 00 01
 * b0 89 ce 7c 1a 35 5d b8 b4 87 ef c0 bc 20 67 d5   <--- last remainder of the cmd 05 packet
 * 09 b0 4e c4 c0 02 fc 45 c8 3e 79 cb 21 17 66 d0
 *
 *
 * It's not clear if it's a problem from the firmware, the USB controller or the
 * pyusb library, but after a connection/deconnection to the device, it doesn't
 * receive nothing anymore (no UIV_EP1OUT irq is triggered).
 *
 */
#include "usb.h"
#include "regs.h"
#include "regs_io.h"
#include "utils.h"
#include "log.h"
#include "brom.h"
#include <exceptions.h>


#define DUMP_REG(r) log(#r ": %x\n", *(r))
#define DUMP8REG(r) log(#r ": %hhx\n", *(u8*)(r))

/* TODO: move to brom */
/* FIXME: reset and define new USB device */
void usb_setup() {
#ifdef DEBUG
    // DUMP_REG(PMU_USB_UNK);
    DUMP8REG(IN2CON); /* this registers need to be accessed ALIGNED and read by 8 byte */
    DUMP8REG(IN2CS);
    DUMP8REG(OUT1CON);
    DUMP8REG(OUT1CS);
    DUMP8REG(USBIEN);
    DUMP8REG(OTGIEN);
    DUMP8REG(OTGIRQ);
    DUMP8REG(OTG_OUT04IEN);
    DUMP8REG(OUT1MAXPCKL);
    DUMP8REG(OUT1MAXPCKH);
    DUMP8REG(OUT1STARTADDRESS);
#endif
    w8(OUT1CS, 0);
    w8(OTG_OUT04IEN, 2);
}

static void usb_handle_otgirq() {
    log("OTGIRQ\n");
    w8(USBEIRQ, 0x88);
    w8(OTGIRQ, 1);
}

static void usb_handle_irq_reset() {
    log("RESET\n");
    w8(USBEIRQ, 0x88);
    w8(USBIRQ, USBIRQ_URES);
}

static void usb_handle_irq_sudav() {
    log("SUDAV\n");
    w8(USBEIRQ, 0x88);
    w8(USBIRQ, USBIRQ_SUDAV);

    struct usb_setup_packet setup_packet;

    u32* p_packet = (u32*)&setup_packet;

    *p_packet++ = r32(SETUPDAT0);
    *p_packet++ = r32(SETUPDAT4);

    hexdump((u8*)&setup_packet, sizeof(struct usb_setup_packet));

    if (!is_device2host(setup_packet)) {
        switch (setup_packet.bRequest) {
            case SET_CONFIGURATION:
                log("SET_CONFIGURATION\n");

                if ((setup_packet.wValue == 0) || (setup_packet.wValue == 1)) {
                    log(" wValue: %x\n", setup_packet.wValue);
                    w8(IN07IRQ,1);
                    w8(EP0CS, EP0CS_HSNAK);
                } else {
                    log("unimplemened SET_CONFIGURATION with wValue != 0 and 1\n");
                }
                break;
            default:
                log("unimplemented Setup case\n");
        }
    } else {
        log("unimplemented device 2 host:P\n");
    }

}


void usb_write_fifo(void* buffer, size_t size) {
    u8  remain = size & 3;
    u32 nwords = (size >> 2) + ((remain) ? 1 : 0);

    u32* ref = (u32*)buffer;

    /*
     * we are going to write into the FIFO
     * aligned to four but we'll send only "size"
     * bytes, in this way is simpler :)
     *
     * It's not going to fail (buffer is read and should be aligned to 4)
     * and it's not leaking.
     */
    while (nwords--) {
        w32(FIFO_IN_DAT, *ref++);
    }

    w8(INxBCL, size & 0xff);
    w8(INxBCH, size >> 8);

    w8(INxCS, 0x0);

    u8 in2cs;
    do {
        in2cs = r8(INxCS);
    } while ((in2cs & EPCS_BUSY) != 0);
}


void usb_handle_ep1out() {
    log("EP1 received packet\n");
    w8(OUT07IRQ, EP1_OUT_IRQ); /*clear ep1out irq */

    u8 sizeL = r8(OUT1BCL);
    u8 sizeH = r8(OUT1BCH);

    u16 size = sizeL + (sizeH << 8);

    if (size != 0x1f) {
        log("enpoint1 received wrong size packet\n");
        return;
    }

    static struct usb_cbw_packet packet;
    u32* ref = (u32*)&packet;

    *ref++ = r32(FIFO_OUT_DAT);
    *ref++ = r32(FIFO_OUT_DAT);
    *ref++ = r32(FIFO_OUT_DAT);
    *ref++ = r32(FIFO_OUT_DAT);
    *ref++ = r32(FIFO_OUT_DAT);
    *ref++ = r32(FIFO_OUT_DAT);
    *ref++ = r32(FIFO_OUT_DAT);
    *ref++ = r32(FIFO_OUT_DAT);

    hexdump(&packet, sizeof(struct usb_cbw_packet));

    /* re-arm for the next OUT transaction */
    w8(OUT1CS, 0x0);
    w8(USBEIRQ, 0x88);

    int reset = 0;

    /* parse the command */
    switch (packet.cmd) {
        case 0xa:
        { /* dump flash */
            log("starting dumping flash\n");
            static u8 buffer[0x200];
            u32 flash_size = 0x10 * 1024 * 1024;
            u32 nsectors = flash_size >> 9;
            for (u32 sector = 0 ; sector < nsectors ; sector++) {
                u32 offset = sector * 0x200;
                flash_read(offset, 0x200, buffer);
                usb_write_fifo(buffer, 0x200);
            }
            break;
        }
        case 0xb:
        { /* dump memory */
            log("dumping memory... ");
            u8* addr = (u8*)packet.arg0;
            size_t length = packet.arg1;

            size_t sent = 0;

            while (sent < length) {
                size_t delta = length - sent;
                size_t packet_size = delta > 0x200 ? 0x200 : delta;

                usb_write_fifo(addr + sent, packet_size);

                sent += packet_size;
            }
            log("done\n");
            break;
        }
        case 0xab: /* dump stack frame */
            log("requested to dump the actual stack frame\n");
            dump_exception_frame(last_frame);
            break;
        case 0xff: /* reset to ADFU */
            log("requested reset board to ADFU mode again\n");
            // TODO: restore Coprocessor registers as in ADFU mode
            //       in the BROM
            reset = 1;
            break;
        default:
            log("CBW command not recognized\n");
    }

    /* answer to it */
    struct usb_csw_packet response = {
        .signature = 0x53425355,
        .tag       = packet.tag,
        .residue   = 0x1337,
        .status    = 0
    };

    ref = (u32*)&response;
    w32(FIFO_IN_DAT, *ref++);
    w32(FIFO_IN_DAT, *ref++);
    w32(FIFO_IN_DAT, *ref++);
    w32(FIFO_IN_DAT, *ref++);

    w8(INxBCL, 0xd);
    w8(INxBCH, 0x0);
    w8(INxCS, 0x0);

    u8 in2cs;
    do {
        in2cs = r8(INxCS);
    } while ((in2cs & EPCS_BUSY) != 0);

    if (reset)
        reset_to_adfu();

    return;
}

void usb_handle_irq() {
    log("USB IRQ raised\n");

    uint8_t ivect = r8(IVECT);

    switch (ivect) {
        case UIV_OTGIRQ:
            usb_handle_otgirq();
            break;
        case UIV_USBRESET:
            usb_handle_irq_reset();
            break;
        case UIV_SUDAV:
            usb_handle_irq_sudav();
            break;
        case UIV_EP1OUT:
            usb_handle_ep1out();
            break;
        default:
            log("unrecognized IVECT entry: %hhx\n", ivect);

            w8(USBEIRQ, 0x88);
            w8(OTGIRQ, 1);
    };

    return;
}

