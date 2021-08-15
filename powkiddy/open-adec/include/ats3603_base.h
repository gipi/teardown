#ifndef __REGS_BASE_H__
#define __REGS_BASE_H__


#define UART_BASE      0xb0168000
#define OTG_BASE       0xb0290000


#define PMU_VDD_CTL    HWREG(0xb0000010)
#define PMU_CTL1       HWREG(0xb0000018)
#define PMU_UNKNOWN    HWREG(0xb0000020)
#define PMU_USB_UNK    HWREG(0xb000002c)

#define CMU_DEVCLKEN   HWREG(0xb0010080)
#define CMU_DEVRST     HWREG(0xb0010084)
#define CMU_FOO        HWREG(0xb001008c)
#define CMU_UART1CLK   HWREG(0xb0010048)

#define CMU_COREPLL    HWREG(0xb0010000)
#define CMU_SDRCLK     HWREG(0xb0010010)

#define GPIO_MFCTL1    HWREG(0xb01c001c)

#endif
