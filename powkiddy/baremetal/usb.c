/*
 * Simple binary to test the USB OTG.
 *
 * To test the correct compilation you can use
 *
 *   $ mips-linux-gnu-objdump -D usb.bin -EL -b binary -m mips:4000
 *
 * Remember that you need to encrypt it in order to be executed by the BROM.
 *
 */
#include <stdint.h>
#include <unistd.h>

#include "regs.h"

void write8(uint32_t* reg, uint8_t value) {
    *(volatile uint8_t*)(reg) = value;
}

void write32(uint32_t* reg, uint32_t value) {
    *(volatile uint32_t*)reg = value;
}

uint8_t read8(uint32_t* reg) {
    return *(volatile uint8_t*)reg;
}

uint32_t read32(uint32_t* reg) {
    return *(volatile uint32_t*)reg;
}

#define serial_write(c)  (*SERIAL_TX = (c))
#define is_serial_free() ((*SERIAL_STAT & (1 << TFFU)) == FIFO_EMPTY)

void delay(unsigned int count);
void serial_init();
void printString(const char*);
void printByte(char);

/*
 * Return the hex of a byte
 */
uint16_t hex(uint8_t c) {
    static char table[0x10] = {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'a', 'b',
        'c', 'd', 'e', 'f'
    };
    uint8_t low  = c & 0x0f;
    uint8_t high = (c & 0xf0) >> 4;

    return (table[low] << 8) | (table[high] << 0);
}

/*
 * Simple function to hexdump stuff. Since stack space
 * is a luxury a fixed space of 32 bytes is used and dumped
 * each time
 */
#define CHUNK_SIZE 16
#define TMP_SIZE   ((CHUNK_SIZE * 2) + 1)

void hexdump(uint8_t* buffer, size_t size) {
    char tmp[TMP_SIZE]; /* 32 + NULL byte */

    /* 
     * we have 32bytes but since each hex needs two bytes
     * we can only decode 16 bytes at the time
     */
    unsigned int count = size >> 4;
    unsigned int remain = size & 0xf;

    while (count--) {
        unsigned int n = 0;
        char* ref = tmp;
        for (n = 0 ; n < CHUNK_SIZE; n++) {
            *(uint16_t*)ref = hex(*buffer++);
            ref += 2;
        }
        *ref = '\0'; /* terminate the temporary buffer */

        printString(tmp); /* dump the string so we can empty it afterwards */
        printByte('\n');
    }

    char* ref = tmp;
    while (remain--) {
        *(uint16_t*)ref = hex(*buffer++);
        ref += 2;
    }

    *ref = '\0';

    printString(tmp);
    printByte('\n');
}

#define LOG(s) do { \
        printString(__FUNCTION__); printString("():"); \
        printString((s)); \
    } while(0)

int wait_endpoint_out0() {
    unsigned int counter = 0;
    while ((*OUT1CS & EPCS_BUSY) && (counter++ < 0x4000000));

    return read32(OUT1CS) & EPCS_BUSY;
}

/* arm the endpoint for the next IN transaction */
void endpoint_out0_arm() {
    write8(OUT1CS, 0);
}

/*
 * Read from the endpoint 1's fifo size bytes into a buffer.
 */
int usb_read_fifo(uint32_t* buffer, size_t size) {
    LOG("\n");

    unsigned int n = 0; /* final number of bytes read */

    unsigned int size_l = read8(OUT1BCL);
    unsigned int size_h = read8(OUT1BCH);

    unsigned int actual_size = size_l + (size_h << 0x8);

    unsigned int count = actual_size >> 2; /* # of words */
    unsigned int remain = actual_size & 0x3; /* # of bytes */

    while (!(read8(OUT1CS) & EPCS_BUSY)) {
        while (count--) {
            *buffer++ = read32(FIFO1DAT);
            n += 4;
        }

        uint8_t* buff8 = (uint8_t*)buffer;
        uint8_t* fifo8 = (uint8_t*)FIFO1DAT;

        while (remain--) {
            *buff8++ = read8(fifo8++);
            n++;
        }

        endpoint_out0_arm();
    }

    return n;
}

int usb_write_fifo(uint32_t* buffer, size_t size) {
    LOG("\n");
    unsigned int n = 0;
    unsigned int count = size >> 2; /* # of words */
    unsigned int remain = size & 0x3; /* # of bytes */

    if (remain > 0) {
        count++; /* write the entire word the important is the byte count register value */
    }

    while (!(read8(IN2CS) & EPCS_BUSY)) {
        while (count--) {
            write32(FIFO2DAT, *buffer++);
            n += 4;
        }
    }

    write8(IN2BCL, size & 0xff);
    write8(IN2BCH, size >> 8);

    write8(IN2CS, 0);
    return n;
}

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

void delay(unsigned int count) {
    while(count-- > 0);
}

struct CBW_t {
    /*  0 */ uint32_t signature;
    /*  4 */ uint32_t tag;
    /*  8 */ uint32_t transferLength;
    /*  c */ uint8_t  flags;
    /*  d */ uint8_t  LUN;
    /*  e */ uint8_t  cmdLength;
    /*  f */ uint8_t  cmd;
    /* 10 */ uint32_t arg0;
    /* 14 */ uint32_t arg1;
    /* 18 */ uint32_t arg2;
    /* 1c */ uint32_t padding;
};

struct CBS_t {
    uint32_t signature;
    uint32_t tag;
    uint32_t dataResidue;
    uint32_t status; /* this is 1 byte, the remaining is padding */
};

/* used for the responses */
static struct CBS_t cbs = {
    .signature   = 0x53425355,
    .tag         = 0,
    .dataResidue = 0,
    .status      = 0
};

void usb_send_response(uint8_t response) {
    LOG("sending response\n");

    cbs.status = response;

    unsigned int n = usb_write_fifo(&cbs, 13);

    hexdump(n, 4);
}

/*
 * Wait for a CBW packet and do something based on the command in it.
 */
void handle_cbw() {
    LOG("receiving a CBW packet\n");
    unsigned int size_l = read8(OUT1BCL);
    unsigned int size_h = read8(OUT1BCH);

    unsigned int size = size_l + (size_h << 0x8);

    if (size != 0x1f) {
        LOG("CBW: wrong size\n");
        return;
    }

    struct CBW_t cbw;
    uint32_t* p_cbw = &cbw;

    /* copy only 28 bytes since the remaining are padding */
    for (unsigned int count = 0 ; count < 7; count++) {
        *p_cbw++ = read32(FIFO1DAT);
    }

    uint8_t response = 0;

    /* decide what to do based on cmd */
    switch (cbw.cmd) {
        case 0x17:
            break;

        default:
            LOG("error: unrecognized command\n");
            response = 0xff;
    }

    hexdump(&cbw, sizeof(cbw));
    //printString("maio");

    endpoint_out0_arm();

    usb_send_response(response);

#if 0
    LOG("OUT1CS: ");
    hexdump(OUT1CS, 1);
    LOG("\n");
#endif

}

char looper[4] = "/|\\-";

/* must be the first function in order to be the on top */
int main(void) {
    static char msg[] = "USB dumping tool\n";
    setup();
    serial_init();
    LOG(msg);

    endpoint_out0_arm(); /* removing this the packet itself is recognized */

    unsigned idx = 0;

    while (1) {
        printByte(looper[((idx++) >> 10) % 4]);
        printByte('\r');
        uint8_t out1cs = read8(OUT1CS);
        //if (out1cs & EPCS_BUSY)
        //    continue;
#if 1
        if (out1cs & EPCS_ERR) {
            LOG("OUT1CS errored!!!\n");
            while(1);
        }
#endif

        if (!(out1cs & EPCS_BUSY)) {
            handle_cbw();
        }
    }
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

