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

#define ADDRESS_BROM_ADFU 0xbfc00110

#define ADDRESS_BROM_NAND_START (u32*)0xbfc03864
#define LENGTH_BROM_NAND        0xfa0

#define ADDRESS_DEST_BROM_NAND  (u32*)0xb4040400

void (*reset_to_adfu)() = (void(*)())ADDRESS_BROM_ADFU;

void flash_read(u32 offset, size_t size, void* buffer) {
}

/*
 * The BROM uses an internal remapped block to access the NAND and look for the
 * Brec stuff. We are going to reuse it for our own purposes.
 */
void map_setup() {
    memcpy(ADDRESS_DEST_BROM_NAND, ADDRESS_BROM_NAND_START, LENGTH_BROM_NAND);
}
