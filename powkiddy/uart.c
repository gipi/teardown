/*
 * Simple binary to test the UART.
 *
 * To test the correct compilation you can use
 *
 *   $ mips-linux-gnu-objdump -D uart.bin -EL -b binary -m mips:4000
 *
 * Remember that you need to encrypt it in order to be executed by the BROM.
 *
 * The UART1_TX pin is the 128 for the ATS3203.
 */
#include <stdint.h>
#include <unistd.h>


#define ATSREG(reg) ((uint32_t*)(reg))
/* 
 * we can't use global variables because we haven't any place to use for them (FOR NOW!).
 */
#define GPIO_MFCTL1    ATSREG(0xb01c001c)
#define CMU_DEVCLKEN   ATSREG(0xb0010080)
#define CMU_DEVRST     ATSREG(0xb0010084)
#define CMU_FOO        ATSREG(0xb001008c)
#define UART1_CLK      ATSREG(0xb0010048)

#define CMU_COREPLL    ATSREG(0xb0010000)
#define CMU_SDRCLK     ATSREG(0xb0010010)
#define PMU_VDD_CTL    ATSREG(0xb0000010)
#define PMU_CTL1       ATSREG(0xb0000018)
#define PMU_UNKNOWN    ATSREG(0xb0000020)

#define U1EN           16
#define TFFU           6 /* TX FIFO Full */

#define FIFO_FULL      1
#define FIFO_EMPTY     0

#define UART_BASE      (0xb0168000)

#define SERIAL_CTL     ATSREG(UART_BASE + 0x00)
#define SERIAL_TX      ATSREG(UART_BASE + 0x08)
#define SERIAL_STAT    ATSREG(UART_BASE + 0x0c)


#define serial_write(c)  (*SERIAL_TX = (c))
#define is_serial_free() ((*SERIAL_STAT & (1 << TFFU)) == FIFO_EMPTY)

void delay(unsigned int count);
void serial_init();
void printString(char*);
void printByte(char);
void printMemory(char* addr);//, size_t length);


/*
 * Pretty much copy-pasted from the reversing of adec09_1.bin.
 *
 * I have no idea of what this stuff is doing if not reading
 * datasheets of similar chip and believing that the registers
 * are the same.
 */
void setup() {
    // this line below skews the clock,
    // the original adec's code sets this register
    // after having checked some values at bfc00012
    //*PMU_UNKNOWN = (*PMU_UNKNOWN & 0xffffff7f) | 0x80;
    *PMU_VDD_CTL = 0xdaaa00;

    *CMU_SDRCLK = 0x1;

    delay(24000);

    *CMU_COREPLL = 0x2328;

    delay(60000);

    *CMU_SDRCLK = 0x42;

    delay(60000);

    *CMU_DEVCLKEN = 0xe4123f5;

    delay(60000);

    *PMU_CTL1 |= 0x400000;
}

void delay(unsigned int count) {
    while(count-- > 0);
}


/* must be the first function in order to be the on top */
int main(void) {
    static char msg[] = {
        'h', 'e', 'l', 'l', 'o', 0xff, 'w', 'o', 'r', 'l', 'd', '\n', '\0'
    };
    setup();
    serial_init();
    uint8_t c = 0;
    while(1) {
        printByte(c++ & 0x7f);
        delay(1000000);
    }
    printString(msg);
#if 0
    size_t count = 0;
    while (count < 0x100) {
        printByte(count++);
    }
    printMemory(NAND_ADDR);
#endif
    return 0;
    //while(1);
}

void set_baudrate(uint32_t baudrate, uint32_t clock) {
    *UART1_CLK = (uint16_t)((clock * 1000000UL)/(baudrate * 8UL)) - 1;
}

/*
 * This appears to be necessary from the datasheet but the serial
 * works anyway if this it's not invoked.
 */
void enable_uart() {
    *UART1_CLK |= (1 << U1EN); /* this seems to be missing from the brec code */
}


/*
 * UART configuration.
 *
 * Some registers are unknown, simply copied from reversing
 * the powkiddy brec code.
 */
inline void serial_init() {
    *CMU_DEVRST = *CMU_DEVRST | 0x10;
    *CMU_FOO = (*CMU_FOO & 0xffffffef) | 0x10;
    *GPIO_MFCTL1 = (*GPIO_MFCTL1 & 0xffff9fff) | 0x80002000;
    *SERIAL_CTL = (1 << 15) | (1 << 1) | (1 << 0); /* enable UART1, 8bit, 1 stop */

    set_baudrate(115200, 24);
}

void printByte(char c) {
    while (!is_serial_free());
    serial_write(c);
}

void printMemory(char* addr) {//, size_t length) {
    size_t count = 0;

    while (1) {
        printByte(*addr);
        addr++;
        count++;
    }
}

void printString(char* buffer) {
    char* ptr = buffer;

    while (*ptr) {
        printByte(*ptr);
        ptr++;
    }
}

