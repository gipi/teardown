/*
 * Overwrite the Exceptions/IRQs handling.
 */
#include <stdint.h>
#include <unistd.h>

#include "regs.h"

#define serial_write(c)  (*SERIAL_TX = (c))
#define is_serial_free() ((*SERIAL_STAT & (1 << TFFU)) == FIFO_EMPTY)

void serial_init();
void delay(unsigned int count) {
    while(count-- > 0);
}
#define LOG(s) do { \
        printString(__FUNCTION__); printString("():"); \
        printString((s)); \
    } while(0)


/*
 * Pretty much copy-pasted from the reversing of adec09_1.bin.
 *
 * I have no idea of what this stuff is doing if not reading
 * datasheets of similar chips and believing that the registers
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

/*
 * UART configuration.
 *
 * Some registers are unknown, simply copied from reversing
 * the powkiddy brec code.
 */
void set_baudrate(uint32_t baudrate, uint32_t clock) {
    *UART1_CLK = (uint16_t)((clock * 1000000UL)/(baudrate * 8UL)) - 1;
}
inline void serial_init() {
    *CMU_DEVRST = *CMU_DEVRST | 0x10;
    *CMU_FOO = (*CMU_FOO & 0xffffffef) | 0x10;
    *GPIO_MFCTL1 = (*GPIO_MFCTL1 & 0xffff9fff) | 0x80002000;
    *SERIAL_CTL = (1 << 15) | (1 << 1) | (1 << 0); /* enable UART1, 8bit, 1 stop */

    set_baudrate(115200, 24);
    //enable_uart();
}

void printByte(char c) {
    while (!is_serial_free());
    serial_write(c);
}

void printString(const char* buffer) {
    char* ptr = buffer;

    while (*ptr) {
        printByte(*ptr);
        ptr++;
    }
}

/* must be the first function in order to be the on top */
int main(void) {
    static char msg[] = "Exceptions/IRQ handling\n";
    setup();
    serial_init();
    LOG(msg);

    while (1);
}
