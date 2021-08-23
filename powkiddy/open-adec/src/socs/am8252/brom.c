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
#include <brom.h>
#include <utils.h>
#include <assert.h>

#define ADDRESS_BROM_ADFU 0xbfc00110

#define ADDRESS_BROM_NAND_START (u32*)0xbfc03864
#define LENGTH_BROM_NAND        0xfa0

#define ADDRESS_DEST_BROM_NAND  0xb4040400

static void (*flash_reset)() = (void (*)()) 0xb4041204;
static void (*flash_init)() = (void (*)()) 0xb40406c0;


// FIXME: the jump address is hardcoded
#define _flash_cmd(cmd, arg0, arg1) \
    asm volatile (                  \
        "li $t0, %[CMD]\n\t"       \
        "li $a0, %[ARG0]\n\t"       \
        "li $a1, %[ARG1]\n\t"       \
        "jal 0xb4040400\n\t"        \
        "nop\n\t"                   \
        :                           \
        : [CMD] "i" (cmd), [ARG0] "ir" (arg0), [ARG1] "i" (arg1) \
        : "ra"                       \
    )

/*
 * A specific macro is needed since the address of the NAND transaction must be
 * passed by register and so it needs a "move" instead of an "lui".
 */
// FIXME: the jump address is hardcoded
#define _flash_read(arg0, arg1) \
    asm volatile (                  \
        "li $t0, %[CMD]\n\t"       \
        "move $a0, %[ARG0]\n\t"     \
        "li $a1, %[ARG1]\n\t"       \
        "jal 0xb4040400\n\t"        \
        "nop\n\t"                   \
        :                           \
        : [CMD] "i" (READ), [ARG0] "r" (arg0), [ARG1] "i" (arg1) \
        : "ra"                      \
    )


enum flash_cmd_t {
    INIT = 1,
    SETUP = 2,
    READ = 4,
    RESET = 0xb
};


/* this internal routine uses $t0 as first parameter */

void (*reset_to_adfu)() = (void(*)())ADDRESS_BROM_ADFU;

struct nand_transaction_t {
    u16 main_col_addr;
    u16 row_addrlo;
    u32 unknown;
    u32* address;
};

/*
 * To read from flash you must call the BROM's internal routine with an
 * appropriate value in $t0 and the address of a particular data structure in
 * $a0; $a1 instead must be 0 (It's not clear the meaning of this parameter).
 *
 * The number of bytes read is fixed in the routine to be 0x200 so an ASSERT is
 * set to guard this assumption.
 */
void flash_read(u32 offset, size_t size, void* buffer) {
    ASSERT(size == 0x200);
    /* configure the data structure to read*/
    struct nand_transaction_t tx = {
        .main_col_addr = offset & 0xffff,
        .row_addrlo = offset >> 16,
        .unknown = 0,
        .address = buffer
    };
    _flash_read(&tx, 0);
}

/*
 */
#define NOP                    0x00000000

void nop_out(u32* start, u32* end) {

    while (start <= end) {
        *start++ = NOP;
    }
}

/*
 * The BROM uses an internal remapped block to access the NAND and look for the
 * Brec stuff. We are going to reuse it for our own purposes.
 *
 * The block is 0xb4040400 - 0xb404139f.
 *
 * There is also a remapping of 0x40 bytes of 0xbfc01cc8 into 0xb4042000 used as
 * BSS where the NAND transactions are described but it's not necessary since
 * it's possible to pass out own data to flash_read().
 *
 * VERY IMPORTANT:
 *
 * the flash_init() function messes up the GPIOs and CMU_DEVRST disabling the USB and UART
 * so some patching is needed in order to fix things.
 *
 * See hal.c:hw_register_setup() for the gpio configuration code.
 */
#define ADDRESS_START_PATCHING_GPIO (u32*)0xb4040748
#define ADDRESS_END_PATCHING_GPIO   (u32*)0xb4040788

#define ADDRESS_START_PATCHING_DEVRST (u32*)0xb40406c0
#define ADDRESS_END_PATCHING_DEVRST   (u32*)0xb40406d8

void map_setup() {
    log("mapping BROM's code\n");
    memcpy((void*)ADDRESS_DEST_BROM_NAND, ADDRESS_BROM_NAND_START, LENGTH_BROM_NAND);

    log("patching it\n");
    nop_out(ADDRESS_START_PATCHING_GPIO, ADDRESS_END_PATCHING_GPIO);
    nop_out(ADDRESS_START_PATCHING_DEVRST, ADDRESS_END_PATCHING_DEVRST);

    log("NAND ");

    flash_reset();
    log(".");
    flash_init();
    log(".");
    _flash_cmd(SETUP, 0x1, 0);
    log(".");
    flash_reset();
    log(".");

    log("ok\n");
}
