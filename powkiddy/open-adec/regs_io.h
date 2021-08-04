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
#include <stdint.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef uint32_t* reg;

u8 r8(reg);
u16 r16(reg);
u32 r32(reg);

void w8(reg, u8);
void w16(reg, u16);
void w32(reg, u32);
