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
#include "regs_io.h"
inline u8 r8(reg r) {
    return *(u8*)r;
}

inline u16 r16(reg r) {
    return *(u16*)r;
}

inline u32 r32(reg r) {
    return *r;
}

inline void w8(reg r, u8 value){
    *(volatile u8*)r = value;
}

inline void w16(reg r, u16 value) {
    *(volatile u16*)r = value;
}

inline void w32(reg r, u32 value) {
    *(volatile u32*)r = value;
}
