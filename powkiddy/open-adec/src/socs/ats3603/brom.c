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
/*
 * Routines that interact with the BROM.
 *
 * NOTE: this is all code ATS3603 specific.
 */
#include "brom.h"
#include "utils.h"
#include "log.h"

#define ADDRESS_BROM_ADFU ((u32*)0xbfc001c0)

#define ADDRESS_BROM_SETUP ((u32*)0xbfc073fc)
#define SIZE_SETUP    0x800

#define ADDRESS_DEST_SETUP ((u32*)0xb4060400)

static void (*_setup)(u32 address, size_t size, void* buffer) = (void (*)())ADDRESS_DEST_SETUP;
void (*reset_to_adfu)() = (void(*)())ADDRESS_BROM_ADFU;

/*
 * Into the BROM exists a block of code that is used to interact (minimally)
 * with the flash. With this routine we copy it to the designed place in memory
 * and use it. It is also needed some initialization.
 */
void map_setup() {
    log("mapping BROM setup()\n");
    memcpy(ADDRESS_DEST_SETUP, ADDRESS_BROM_SETUP, SIZE_SETUP);

    /* magic initialization (otherwise the flash_read() doesn't work correctly) */
    asm volatile("li $t0, 0x01");
    _setup(0, 0, 0);
}

/*
 * This function reads a certain amount of flash into a buffer.
 *
 * Note: the internal BROM's function takes an extra argument via the t0
 * register that must be set to 2.
 */
void flash_read(u32 address, size_t size, void* buffer) {
    asm volatile("li $t0, 0x02");

    _setup(address, size, buffer);
}
