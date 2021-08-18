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
#ifndef __REGS_BASE_H__
#define __REGS_BASE_H__


#define UART_BASE      0xb0190020
#define OTG_BASE       0xb00e0000

/* the Mirascreen ADFU mode configure endpoint 1 as IN and OUT */
#define INxBCL         IN1BCL
#define INxBCH         IN1BCH
#define INxCS          IN1CS

/* and it's using the same FIFOxDAT0 to receive/transmit data */
#define FIFO_IN_DAT    FIFO1DAT0
#define FIFO_OUT_DAT   FIFO1DAT0

#define CMU_BASE       0xb0010000
#define SDR_BASE       0xb0070000
#define GPIO_BASE      0xb01c0000


//--------CMU/HOSC--------------------------------------------------
#define     CMU_COREPLL                          HWREG(CMU_BASE+0x00)
#define     CMU_HOSC                             HWREG(CMU_BASE+0x04)
#define     CMU_AUDIOPLL                         HWREG(CMU_BASE+0x08)
#define     CMU_BUSCLK                           HWREG(CMU_BASE+0x0C)
#define     CMU_HCMUTEST                         HWREG(CMU_BASE+0x10)
#define     CMU_NANDCLK                          HWREG(CMU_BASE+0x18)
#define     CMU_SDIOCLK                          HWREG(CMU_BASE+0x1C)

#define     CMU_UART1CLK                         HWREG(CMU_BASE+0x28)
#define     CMU_UART2CLK                         HWREG(CMU_BASE+0x2C)
#define     CMU_DMACLK                           HWREG(CMU_BASE+0x30)
#define     CMU_FM_PMUCLK                        HWREG(CMU_BASE+0x34)
#define     CMU_SPCLK                            HWREG(CMU_BASE+0x38)

#define     CMU_DEVCLKEN2                        HWREG(CMU_BASE+0x7C)
#define     CMU_DEVCLKEN                         HWREG(CMU_BASE+0x80)
#define     CMU_DEVRST                           HWREG(CMU_BASE+0x84)
#define     CMU_DEVRST2                          HWREG(CMU_BASE+0x88)
#define     CMU_LCDPLL                           HWREG(CMU_BASE+0x90)
#define     CMU_CARDCLK                          HWREG(CMU_BASE+0x94)
#define     CMU_DISPLAYCLK                       HWREG(CMU_BASE+0x98)
#define     CMU_CARDCLKSEL                       HWREG(CMU_BASE+0x9C)
#define     PWM0_CTL                             HWREG(CMU_BASE+0xA0)
#define     PWM1_CTL                             HWREG(CMU_BASE+0xA4)
#define     CMU_TROMCLK                          HWREG(CMU_BASE+0xA8)
#define     CMU_DISPLAYCLK2                      HWREG(CMU_BASE+0xAC)
#define     CMU_COREPLL2                         HWREG(CMU_BASE+0xB0)
#define     CMU_DDRPLL                           HWREG(CMU_BASE+0xB4)
#define     CMU_DDRPLL2                          HWREG(CMU_BASE+0xB8)
#define     CMU_EMMCCLK                          HWREG(CMU_BASE+0xC0)
#define     CMU_EMMCCLKSEL                       HWREG(CMU_BASE+0xC4)

//--------SDRAM/DDR2 CONTROLLER-    -----------------------------------
#define     SDR_CTL                              HWREG(SDR_BASE+0x00)

#define     SDR_EN                               HWREG(SDR_BASE+0x08)
#define     SDR_CMD                              HWREG(SDR_BASE+0x0C)
#define     SDR_STAT                             HWREG(SDR_BASE+0x10)
#define     SDR_RFSH                             HWREG(SDR_BASE+0x14)
#define     SDR_MODE                             HWREG(SDR_BASE+0x18)
#define     SDR_EMODE1                           HWREG(SDR_BASE+0x1C)
#define     SDR_EMODE2                           HWREG(SDR_BASE+0x20)
#define     SDR_EMODE3                           HWREG(SDR_BASE+0x24)
#define     SDR_WEIGHT                           HWREG(SDR_BASE+0x54)
#define     SDR_WEIGHT2                          HWREG(SDR_BASE+0xB0)
#define     SDR_CLKDLY                           HWREG(SDR_BASE+0x5C)
#define     SDR_INITD                            HWREG(SDR_BASE+0x28)
#define     SDR_SR                               HWREG(SDR_BASE+0x2C)
#define     SDR_TIMING                           HWREG(SDR_BASE+0x30)
#define     SDR_PD                               HWREG(SDR_BASE+0x34)
#define     SDR_UNKNOWN                          HWREG(SDR_BASE+0x38)

#define     SDR_LATENCY                          HWREG(SDR_BASE+0x58)
#define     SDR_MSTDLY                           HWREG(SDR_BASE+0x60)
#define     SDR_TIMING2                          HWREG(SDR_BASE+0x64)
#define     SDR_ZQTIMING                         HWREG(SDR_BASE+0x68)
#define     SDR_ZQCS                             HWREG(SDR_BASE+0x6C)

#define     SDR_VERSION                          HWREG(SDR_BASE+0x3C)
#define     SDR_WCNT                             HWREG(SDR_BASE+0x40)
#define     SDR_RCNT                             HWREG(SDR_BASE+0x44)
#define     SDR_CAPCLK                           HWREG(SDR_BASE+0x48)

#define     SDR_ADDRSWAP                         HWREG(SDR_BASE+0x4C)
#define     SDR_PRIORITY                         HWREG(SDR_BASE+0x50)
#define     SDR_PRIORITY2                        HWREG(SDR_BASE+0xAC)

#define     SDR_RESETD                           HWREG(SDR_BASE+0x70)
#define     SDR_PADCTRL1                         HWREG(SDR_BASE+0x74)
#define     SDR_PADCTRL2                         HWREG(SDR_BASE+0x78)
#define     SDR_PADCTRL3                         HWREG(SDR_BASE+0x7C)

//--------GPIO-----------------------------------------------
#define     GPIO_31_0OUTEN                       HWREG(GPIO_BASE+0x00)
#define     GPIO_31_0INEN                        HWREG(GPIO_BASE+0x04)
#define     GPIO_31_0DAT                         HWREG(GPIO_BASE+0x08)
#define     GPIO_63_32OUTEN                      HWREG(GPIO_BASE+0x0C)
#define     GPIO_63_32INEN                       HWREG(GPIO_BASE+0x10)
#define     GPIO_63_32DAT                        HWREG(GPIO_BASE+0x14)
#define     GPIO_95_64OUTEN                      HWREG(GPIO_BASE+0x18)
#define     GPIO_95_64INEN                       HWREG(GPIO_BASE+0x1C)
#define     GPIO_95_64DAT                        HWREG(GPIO_BASE+0x20)

#define     LVDS_PAD_CFG0                        HWREG(GPIO_BASE+0x30)
#define     LVDS_PAD_CFG1                        HWREG(GPIO_BASE+0x34)

#define     GPIO_MFCTL0                          HWREG(GPIO_BASE+0x40)
#define     GPIO_MFCTL1                          HWREG(GPIO_BASE+0x44)
#define     GPIO_MFCTL2                          HWREG(GPIO_BASE+0x48)
#define     GPIO_MFCTL3                          HWREG(GPIO_BASE+0x4c)
#define     GPIO_MFCTL4                          HWREG(GPIO_BASE+0x50)
#define     GPIO_MFCTL5                          HWREG(GPIO_BASE+0x54)
#define     GPIO_MFCTL6                          HWREG(GPIO_BASE+0x58)
#define     GPIO_MFCTL7                          HWREG(GPIO_BASE+0x5C)
#define     GPIO_MFCTL8                          HWREG(GPIO_BASE+0x60)

#define     MU_REG1                              HWREG(GPIO_BASE+0x80)

#define     MU_REG2                              HWREG(GPIO_BASE+0x90)
#define     PH_REG                               HWREG(GPIO_BASE+0x94)

#define     SDR_PD_REG                           HWREG(GPIO_BASE+0xA0)
#define     SDR_MU_REG                           HWREG(GPIO_BASE+0xA4)

#define     NOR_ADDR_OE                          HWREG(GPIO_BASE+0xB0)

#define     P_TEST_IE                            HWREG(GPIO_BASE+0xB8)
#define     HDMI_DEBUG                           HWREG(GPIO_BASE+0xBC)

#define     DEBUG_CON                            HWREG(GPIO_BASE+0xC0)

#define     KDS_IN_PU                            HWREG(GPIO_BASE+0xD0)

#endif
