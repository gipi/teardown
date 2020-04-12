#include <stdint.h>
#include <unistd.h>


/* 
 * we can't use global variables because we haven't any place to use for them.
 */
#define SERIAL_CTL     ((uint32_t*)0xb0190020)
#define SERIAL_ADDRESS ((uint32_t*)0xb0190028)
#define SERIAL_REG     ((uint32_t*)0xb019002c)
#define NAND_ADDR      ((uint32_t*)(0xa0000000))
#define ADDR_SECRET    ((uint32_t*)(0xa00296fc))
#define LENGTH_SECRET  (0x90)


#define serial_write(c) (*SERIAL_ADDRESS = (c))
#define is_serial_free() ((*SERIAL_REG >> 6) & 1)

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
#endif
    printMemory(NAND_ADDR);
}

inline void serial_init() {
    *SERIAL_CTL = 0xc8403;
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

