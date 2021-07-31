#ifndef __REGS_H__
#define __REGS_H__

#define ATSREG(reg) ((uint32_t*)(reg))

#define PMU_VDD_CTL    ATSREG(0xb0000010)
#define PMU_CTL1       ATSREG(0xb0000018)
#define PMU_UNKNOWN    ATSREG(0xb0000020)

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

#define UART_BASE      (0xb0168000)

#define SERIAL_CTL     ATSREG(UART_BASE + 0x00)
#define SERIAL_TX      ATSREG(UART_BASE + 0x08)
#define SERIAL_STAT    ATSREG(UART_BASE + 0x0c)


/**********
     USB
***********/
#define OTG_BASE       0xb0290000
#define OTGREG(v)      ((uint32_t*)(OTG_BASE + v))

#define OUT1BCL        OTGREG(0x0008)
#define OUT1BCH        OTGREG(0x0009)
#define OUT1CS         OTGREG(0x000b)

#define IN2BCL         OTGREG(0x0014)
#define IN2BCH         OTGREG(0x0015)
#define IN2CS          OTGREG(0x0017)

#define FIFO1DAT       OTGREG(0x0084)
#define FIFO2DAT       OTGREG(0x0088)

// MASKS FOR endpoints CS
#	define	EPCS_AUTO_IN		(1<<4)
#	define	EPCS_AUTO_OUT		(1<<4)
#	define	EPCS_BUSY			(1<<1)
#	define	EPCS_ERR				(1<<0)
#endif
