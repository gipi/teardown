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
#ifndef __UTILS_H__
#define __UTILS_H__
#include "regs_io.h"
#include "ex_frame.h"
#include <stddef.h>

enum hd_mode {
    HD_MODE_8 = 0,
    HD_MODE_16 = 1,
    HD_MODE_32 = 3
};

char* int2hex(u32 value, enum hd_mode mode);
size_t strlen(const char* str);
void _delay(unsigned int delay);
void hexdump(void* buffer, size_t size);
void dump_exception_frame(struct eframe* ef);
void hd32le(u32 value, void* out);
void* memcpy(void* dest, void* src, size_t size);

#endif
