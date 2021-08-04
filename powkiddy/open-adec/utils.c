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
#include "utils.h"
#include "uart.h"
#include "ex_frame.h"
#include "log.h"

void _delay(unsigned int count) {
    while(count-- > 0);
}


size_t strlen(const char* str) {
    size_t counter = 0;

    while (*str++) {
        counter++;
    }

    return counter;
}
/*
 * Return the hex of a byte
 */
u16 hex(u8 c) {
    char table[0x10] = {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'a', 'b',
        'c', 'd', 'e', 'f'
    };
    u8 low  = c & 0x0f;
    u8 high = (c & 0xf0) >> 4;

    return (table[low] << 8) | (table[high] << 0);
}

/*
 * Simple function to hexdump stuff.
 */
#define CHUNK_SIZE 16

void hexdump(void* buffer, size_t size) {
    static char tmp[3]; /* 2 hex digit plus the terminating null byte*/

    u8* _buffer = (u8*)buffer;
    /* 
     * we have 32bytes but since each hex needs two bytes
     * we can only decode 16 bytes at the time
     */
    unsigned int count = size >> 4;
    unsigned int remain = size & 0xf;

    while (count--) {
        unsigned int n = 0;
        for (n = 0 ; n < CHUNK_SIZE; n++) {
            char* ref = tmp;

            *(u16*)ref = hex(*_buffer++);
            ref += 2;
            *ref = '\0';

            serial_puts(tmp);
            serial_putc(' ');
        }

        serial_putc('\n');
    }

    while (remain--) {
        char* ref = tmp;
        *(u16*)ref = hex(*_buffer++);

        ref += 2;
        *ref = '\0';

        serial_puts(tmp);
        serial_putc(' ');
    }

    serial_putc('\n');
}

/* NOTE: out MUST be at least of size 8 */
void hd32le(u32 value, void* out) {
    u8* p_value = (u8*)&value;
    u16* p_out  = (u16*)out;

    int index;
    for (index = 3 ; index >= 0 ; index--) {
        *(p_out+index) = hex(*p_value++);
    }
}

char* registers_names[32] = {
    "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3",
    "t4", "t5", "t6", "t7",
    "t8", "t9", "s0", "s1",
    "s2", "s3", "s4", "s5",
    "s6", "s7", "fp", "ra",
    "gp", " 0", "v0", "v1",
    "at", "k0", "k1", "sp"
};

#define SPACING "        "

void dump_exception_frame(struct eframe* ef) {
    log("----8<-----------------------\n");
    log("dumping registers:\n");

    u8 tmp_reg[9];
    tmp_reg[8] = '\0';

    /* we are going to dump 8 rows of 4 registers each */
    for (unsigned int y = 0 ; y < 8 ; y++) {
        log(SPACING);
        for (unsigned int x = 0 ; x < 4 ; x++) {
            unsigned int index = x + (4 * y);
            log(registers_names[index]);
            log(": ");

            hd32le(ef->r[index], tmp_reg);
            log(tmp_reg);

            log(SPACING);
        }
        log("\n");
    }

    log("\n");

    char buff[10];
    buff[8] = ' ';
    buff[9] = '\0';

    hd32le(ef->epc, buff);
    log("EPC: ");
    log(buff);

    hd32le(ef->cause, buff);
    log("CAUSE: ");
    log(buff);
    
    hd32le(ef->status, buff);
    log("STATUS: ");
    log(buff);

    hd32le(ef->hi, buff);
    log("Hi: ");
    log(buff);

    hd32le(ef->lo, buff);
    log("LOW: ");
    log(buff);
    log("\n");
    log("--------------------->8------\n");
}

void* memcpy(void* dest, void* src, size_t size) {
    void* result = dest;
    while (size--) {
        *(u8*)dest++ = *(u8*)src++;
    }

    return result;
}
