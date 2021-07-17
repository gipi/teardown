/*
 * Simple binary to test the UART.
 *
 * To test the correct compilation you can use
 *
 *   $ mips-linux-gnu-objdump -D uart.bin -EL -b binary -m mips:4000
 */
#include <stdint.h>
#include <unistd.h>


/* 
 * we can't use global variables because we haven't any place to use for them (FOR NOW!).
 */
#define GPIO_MFCTL1    ((uint32_t*)0xb01c001c)
#define CMU_DEVRST     ((uint32_t*)0xb0010080)
#define CMU_FOO        ((uint32_t*)0xb001008c)
#define UART1_CLK      ((uint32_t*)0xb0010048)

#define U1EN           16


#define UART_BASE      ((uint32_t*)0xb0168000)

#define SERIAL_CTL     ((uint32_t*)UART_BASE + 0x00)
#define SERIAL_TX      ((uint32_t*)UART_BASE + 0x08)
#define SERIAL_STAT    ((uint32_t*)UART_BASE + 0x0c)


#define serial_write(c)  (*SERIAL_TX = (c))
#define is_serial_free() ((*SERIAL_STAT >> 6) & 1)

void serial_init();
void printString(char*);
void printByte(char);
void printMemory(char* addr);//, size_t length);

/* must be the first function in order to be the on top */
void _start(void) {
    char msg[] = {
        'h', 'e', 'l', 'l', 'o', 0xff, 'w', 'o', 'r', 'l', 'd', '\n', '\0'
    };
    serial_init();
    printString(msg);
#if 0
    size_t count = 0;
    while (count < 0x100) {
        printByte(count++);
    }
    printMemory(NAND_ADDR);
#endif
}

void set_baudrate(uint32_t baudrate, uint32_t clock) {
    *UART1_CLK = (clock * 1000000)/(baudrate * 8) - 1;
}

/*
 * UART configuration.
 *
 * Some registers are unknown, simply copied from reversing
 * the powkiddy brec code.
 */
inline void serial_init() {
    *CMU_DEVRST = *CMU_DEVRST | 0x10;
    *CMU_FOO = *CMU_FOO & 0xffffffef | 0x10;
    *GPIO_MFCTL1 = *GPIO_MFCTL1 & 0xffff9fff | 0x80002000;
    *SERIAL_CTL = 0x8003; /* 8bit, 1 stop */

    set_baudrate(115200, 24);
}

void enable_uart() {
    *UART1_CLK |= (1 << U1EN); /* this seems to be missing from the brec code */
}

void printByte(char c) {
    while (is_serial_free());
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

