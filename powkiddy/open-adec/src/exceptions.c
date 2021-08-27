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
#include "exceptions.h"
#include "utils.h"
#include "log.h"

struct eframe* last_frame;

const char* exc_code_description [32] = {
    [0x00] = "INT",     // Interrupt
    [0x01] = "TLBM",    // TLB modification exception
    [0x02] = "TLBL",    // TLB exception (load or instruction fetch)
    [0x03] = "TLBS",    // TLB exception (store)
    [0x04] = "AdEL",    // Address error exception (load or instruction fetch)
    [0x05] = "AdES",    // Address error exception (store)
    [0x06] = "IBE",     // Bus error exception (instruction fetch)
    [0x07] = "DBE",     // Bus error exception (data reference: load or store)
    [0x08] = "SYS",     // Syscall exception
    [0x09] = "BREAK",   // Breakpoint exception
    [0x0a] = "RI",      // Reserved instruction exception
    [0x0b] = "COPRO",   // Coprocessor Unusable exception
    [0x0c] = "OVER",    // Arithmetic Overflow exception
    [0x0d] = "TRAP",    // Trap exception
    [0x0e] = "MSAFP",   // MSA Floating Point exception
    [0x0f] = "FPE",     // Floating point exception
    [0x10] = "?10?",    // ?
    [0x11] = "?11?",    // ?
    [0x12] = "C2E",     // Reserved for precise Coprocessor 2 exceptions
    [0x13] = "TLBRI",   // TLB Read Inhibit exception
    [0x14] = "TLBXI",   // TLB Execution Inhibit exception
    [0x15] = "MSAD",    // MSA Disable exception
    [0x16] = "MDMX",    // MDMX Unusable Exception.
    [0x17] = "WATCH",   // Reference to WatchHi/WatchLo address
    [0x18] = "MCHK",    // Machine check
    [0x19] = "MT",      // MT exception
    [0x1a] = "DSPD",    // DSP Disable exception
    [0x1b] = "GUEST",   // Virtualized Guest exception
    [0x1c] = "?1c?",    // ?
    [0x1d] = "?1d?",    // ?
    [0x1e] = "CERR",    // Cache error
    [0x1f] = "?1f?",    // ?
};

void _handle_exception(struct eframe* ef) {
    u8 cause = (ef-> cause >> 2) & 0x1f;
    log("EXCEPTION\n");
    log(" cause: ");
    log(exc_code_description[cause]);
    log("\n");

    dump_exception_frame(ef);
    log("PANICKING!!!");

    while(1);
}
