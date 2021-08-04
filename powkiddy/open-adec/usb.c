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
#include "usb.h"
#include "regs.h"
#include "regs_io.h"
#include "utils.h"
#include "log.h"


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
#if 0
                    w16(OUT1STARTADDRESS, 0x80);
                    w8(OUT1MAXPCKL, 0x40);
                    w8(OUT1MAXPCKH, 0x00);

                    w8(OTGIEN, 1);
                    w8(OUT1CON, 0x88);
                    w8(ENDPRST, 1);
                    w8(ENDPRST, 0x41);

                    w8(OUT07IRQ, 2);
                    w8(OTG_OUT04IEN,2);
                    w16(OTG_IN2STADDR,0x880);

                    w8(OTG_HCOUT2MAXPCKL,0x40);
                    w8(OTG_HCOUT2MAXPCKH,0);
                    
                    w8(IN2CON,0x88);
                    w8(ENDPRST,0x12);
                    w8(ENDPRST,0x52);
#endif
                    w8(IN07IRQ,1); /* TODO: understand mask here */
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

static void usb_handle_ep1out() {
    log("packet yeah\n");
    w32(OUT07IRQ, EP1_OUT_IRQ); /*clear ep1out irq */

    //u32 signature = r32(FIFO1DAT);
    return;
}

void usb_handle_irq() {

    asm volatile("di");
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
            log("unrecognized IVECT entry: ");
            hexdump(&ivect, 1);

            w8(USBEIRQ, 0x88);
            w8(OTGIRQ, 1);
    };

    asm volatile("ei");
    return;
}

