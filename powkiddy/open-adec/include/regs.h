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
#ifndef __REGS_H__
#define __REGS_H__
#include <stdint.h>
#include "ats3603_base.h"

#define ATSREG(reg) ((uint32_t*)(reg))

#define PMU_VDD_CTL    ATSREG(0xb0000010)
#define PMU_CTL1       ATSREG(0xb0000018)
#define PMU_UNKNOWN    ATSREG(0xb0000020)
#define PMU_USB_UNK    ATSREG(0xb000002c)

#define CMU_DEVCLKEN   ATSREG(0xb0010080)
#define CMU_DEVRST     ATSREG(0xb0010084)
#define CMU_FOO        ATSREG(0xb001008c)
#define UART1_CLK      ATSREG(0xb0010048)

#define CMU_COREPLL    ATSREG(0xb0010000)
#define CMU_SDRCLK     ATSREG(0xb0010010)

#define GPIO_MFCTL1    ATSREG(0xb01c001c)

#define U1EN           16
#define TFFU           6 /* TX FIFO Full */

#define FIFO_FULL      1
#define FIFO_EMPTY     0


#define SERIAL_CTL     ATSREG(UART_BASE + 0x00)
#define SERIAL_TX      ATSREG(UART_BASE + 0x08)
#define SERIAL_STAT    ATSREG(UART_BASE + 0x0c)

/**************
 *
 * RTC
 *
 * ************/

#define RTC_WDCTL      ATSREG(0xb001801c)

/**********
     USB
***********/
#define OTGREG(v)      ((uint32_t*)(OTG_BASE + v))

#define EP0CS          OTGREG(0x0002)

#define OUT1BCL        OTGREG(0x0008)
#define OUT1BCH        OTGREG(0x0009)
#define OUT1CON        OTGREG(0x000a) /* configuration */
#define OUT1CS         OTGREG(0x000b)

#define IN2BCL         OTGREG(0x0014)
#define IN2BCH         OTGREG(0x0015)
#define IN2CON         OTGREG(0x0016)
#define IN2CS          OTGREG(0x0017)

#define FIFO1DAT       OTGREG(0x0084)
#define FIFO2DAT       OTGREG(0x0088)

/* SETUP */
#define SETUPDAT0      OTGREG(0x180) 
#define SETUPDAT1      OTGREG(0x181) 
#define SETUPDAT2      OTGREG(0x182) 
#define SETUPDAT3      OTGREG(0x183) 
#define SETUPDAT4      OTGREG(0x184) 
#define SETUPDAT5      OTGREG(0x185) 
#define SETUPDAT6      OTGREG(0x186) 
#define SETUPDAT7      OTGREG(0x187) 
/*EP0 IRQ*/
#define IN07IRQ        OTGREG(0x188)
#define OUT07IRQ       OTGREG(0x18a)

#define USBIRQ         OTGREG(0x18c)
#define OTG_IN04IEN    OTGREG(0x194)
#define OTG_OUT04IEN   OTGREG(0x196)
#define USBIEN         OTGREG(0x198)


#define IVECT          OTGREG(0x1a0)
#define FIFOVECT       OTGREG(0x1a1)
#define ENDPRST        OTGREG(0x1a2)
#define USBCS          OTGREG(0x1a3)

#define OTGIRQ         OTGREG(0x1bc)
#define OTGIEN         OTGREG(0x1c0)

#define OUT1MAXPCKL    OTGREG(0x1e2)
#define OUT1MAXPCKH    OTGREG(0x1e3)


#define OUT1STARTADDRESS OTGREG(0x304)

#define OTG_IN2STADDR  OTGREG(0x348)

#define OTG_HCOUT2MAXPCKL OTGREG(0x3e4)
#define OTG_HCOUT2MAXPCKH OTGREG(0x3e5)

#define USBEIRQ        OTGREG(0x400)


/*mask for EP0CS*/
#      define       		EP0CS_HCSETTOOGLE  (1<<6)
#      define       		EP0CS_HCCLRTOOGLE  (1<<5)
#      define 			EP0CS_HCSET     (1<<4)
#	define			EP0CS_HCINBSY		(1<<3)
#	define			EP0CS_HCOUTBSY	(1<<2)
#	define			EP0CS_OUTBSY	(1<<3)
#	define			EP0CS_INBSY	(1<<2)
#	define EP0CS_HSNAK         (1<<1)
#	define			EP0CS_STALL	(1<<0)

/*mask for USBIVECT*/
#	define	UIV_SUDAV           	0x00
#	define 	UIV_SOF            	 	0x04
#	define 	UIV_SUTOK           	0x08
#	define 	UIV_SUSPEND        	0x0c
#	define 	UIV_USBRESET        0x10
#	define 	UIV_HSPEED          	0x14
#	define	UIV_HCOUT0ERR	0x16
#	define 	UIV_EP0IN           	0x18
#	define 	UIV_HCIN0ERR		0x1a
#	define 	UIV_EP0OUT          	0x1c
#	define 	UIV_EP0PING         	0x20
#	define	UIV_EP1IN          	0x24
#	define 	UIV_EP1OUT          	0x28
#	define 	UIV_EP1PING         	0x2c
#	define 	UIV_EP2IN           	0x30
#	define 	UIV_EP2OUT          	0x34
#	define 	UIV_EP2PING         	0x38
#	define 	UIV_EP3IN           	0x3c
#	define 	UIV_EP3OUT          	0x40
#	define 	UIV_EP3PING         	0x44
#	define 	UIV_EP4IN           	0x48
#	define 	UIV_EP4OUT          	0x4c
#	define 	UIV_EP4PING         	0x50
#	define 	UIV_OTGIRQ          	0xd8
 
/*mask for USBIRQ*/
#	define	USBIRQ_HS	        	(1<<5)
#	define	USBIRQ_URES			(1<<4)
#	define	USBIRQ_SUSP			(1<<3)
#	define	USBIRQ_SUTOK		(1<<2)
#	define	USBIRQ_SOF			(1<<1)
#	define	USBIRQ_SUDAV		(1<<0)
// MASKS FOR endpoints CS
#	define	EPCS_AUTO_IN		(1<<4)
#	define	EPCS_AUTO_OUT		(1<<4)
#	define	EPCS_BUSY			(1<<1)
#	define	EPCS_ERR				(1<<0)
/*mask for EPXOUTIEN*/
#define	EP6_OUT_IRQ	(1<<6)
#define	EP5_OUT_IRQ	(1<<5)
#	define	EP4_OUT_IRQ	(1<<4)
#	define	EP3_OUT_IRQ	(1<<3)
#	define	EP2_OUT_IRQ	(1<<2)
#	define	EP1_OUT_IRQ	(1<<1)
#	define	EP0_OUT_IRQ	(1<<0)

/*mask for USBEIRQ*/
#	define	USBEIRQ_WUIRQEN	       		(1<<7)
#	define	USBEIRQ_USBIRQEN			(1<<6)
#	define	USBEIRQ_WUIRQ				(1<<5)
#	define	USBEIRQ_USBIRQ				(1<<4)
#	define	USBEIRQ_WUDPEN			(1<<2)
#	define	USBEIRQ_WUIDEN				(1<<1)
#	define	USBEIRQ_WUVBUSEN			(1<<0)

/*mask for USBIEN*/
#	define	USBIEN_HS	        (1<<5)
#	define	USBIEN_URES	(1<<4)
#	define	USBIEN_SUSP	(1<<3)
#	define	USBIEN_SUTOK	(1<<2)
#	define	USBIEN_SOF	(1<<1)
#	define	USBIEN_SUDAV	(1<<0)

#endif
