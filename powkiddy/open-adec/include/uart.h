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
#ifndef __UART_H__
#define __UART_H__
#include "regs.h"

#define serial_write(c)  (*SERIAL_TX = (c))
#define is_serial_free() ((*SERIAL_STAT & (1 << TFFU)) == FIFO_EMPTY)

void serial_init();
void serial_putc(unsigned char c);
void serial_puts(const char*);

#endif
