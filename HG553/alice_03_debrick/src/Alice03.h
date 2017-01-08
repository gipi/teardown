// **************************************************************************
//
//  Alice03.h - Header file for Alice EJTAG Debrick Utility v0.3
//
//  Heavily based on the WRT54G/GS EJTAG Debrick Utility  v4.5/v4.8
//  and on Telsey MAGIC EJTAG Debrick Utility v0.9
//
//  v 0.1 - Telsey Magic, Pirelli Vela and Pirelli AGA support
//  v 0.2 - Added Pirelli AGB support; byteswap corrected
//  v 0.3 - Added Pirelli AGPF support
//
// **************************************************************************
//  Written by 30252783, Janniz, HairyDairyMaid, cattivo12, beghiero 
// **************************************************************************
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of version 2 the GNU General Public License as published
//  by the Free Software Foundation.
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//  more details.
//  To view a copy of the license go to:
//  http://www.fsf.org/copyleft/gpl.html
//  To receive a copy of the GNU General Public License write the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// **************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#ifndef WINDOWS_VERSION

   #include <unistd.h>
   #include <sys/ioctl.h>

   #ifdef __FreeBSD__
      #include <dev/ppbus/ppi.h>
      #include <dev/ppbus/ppbconf.h>
      #define PPWDATA PPISDATA
      #define PPRSTATUS PPIGSTATUS
   #else
      #include <linux/ppdev.h>
   #endif

#endif

#define true  1
#define false 0

#define RETRY_ATTEMPTS 16

// --- Cable Types ---
#define XILINX  0
#define JKEYS   1
#define WIGGLER 2

// --- Some EJTAG Instruction Registers ---
#define INSTR_EXTEST	0x00
#define INSTR_IDCODE    0x01
#define INSTR_SAMPLE    0x02
#define INSTR_IMPCODE   0x03
#define INSTR_ADDRESS   0x08
#define INSTR_DATA      0x09
#define INSTR_CONTROL   0x0A
#define INSTR_BYPASS    0xFF

// --- Some EJTAG Bit Masks ---
#define TOF             (1 << 1 )
#define BRKST           (1 << 3 )
#define DRWN            (1 << 9 )
#define DERR            (1 << 10)
#define DSTRT           (1 << 11)
#define SETDEV          (1 << 14)
#define PROBEN          (1 << 15)
#define DMAACC          (1 << 17)
#define PRACC           (1 << 18)
#define PRNW            (1 << 19)
#define DLOCK           (1 << 5 )
#define TIF             (1 << 2 )
#define SYNC            (1 << 23)
#define PRRST           (1 << 16)
#define PERRST          (1 << 20)
#define JTAGBRK         (1 << 12)
#define DNM             (1 << 28)
#define ROCC            (1 << 31)

#define DMA_BYTE        0x00000000  //DMA tranfser size BYTE
#define DMA_HALFWORD    0x00000080  //DMA transfer size HALFWORD
#define DMA_WORD        0x00000100  //DMA transfer size WORD
#define DMA_TRIPLEBYTE  0x00000180  //DMA transfer size TRIPLEBYTE

#define  size4K			0x1000
#define  size8K			0x2000
#define  size16K		0x4000
#define  size32K		0x8000
#define  size64K		0x10000
#define  size128K		0x20000

#define  size1MB		0x100000
#define  size2MB		0x200000
#define  size4MB		0x400000
#define  size8MB		0x800000
#define  size16MB		0x1000000

#define  CMD_TYPE_BSC  0x01
#define  CMD_TYPE_SCS  0x02
#define  CMD_TYPE_AMD  0x03
#define  CMD_TYPE_SST  0x04

#define  STATUS_READY  0x0080

// EJTAG DEBUG Unit Vector on Debug Break
#define MIPS_DEBUG_VECTOR_ADDRESS           0xFF200200

// Our 'Pseudo' Virtual Memory Access Registers
#define MIPS_VIRTUAL_ADDRESS_ACCESS         0xFF200000
#define MIPS_VIRTUAL_DATA_ACCESS            0xFF200004


// --- Uhh, Just Because I Have To ---
void chip_detect(void);
void chip_shutdown(void);
static unsigned char clockin(int tms, int tdi);
void define_block(unsigned int block_count, unsigned int block_size);
static unsigned int ejtag_read(unsigned int addr);
static unsigned int ejtag_read_h(unsigned int addr);
void ejtag_write(unsigned int addr, unsigned int data);
void ejtag_write_h(unsigned int addr, unsigned int data);
static unsigned int ejtag_dma_read(unsigned int addr);
static unsigned int ejtag_dma_read_h(unsigned int addr);
void ejtag_dma_write(unsigned int addr, unsigned int data);
void ejtag_dma_write_h(unsigned int addr, unsigned int data);
static unsigned int ejtag_pracc_read(unsigned int addr);
void ejtag_pracc_write(unsigned int addr, unsigned int data);
static unsigned int ejtag_pracc_read_h(unsigned int addr);
void ejtag_pracc_write_h(unsigned int addr, unsigned int data);
void identify_flash_part(void);
void lpt_closeport(void);
void lpt_openport(void);
static unsigned int ReadData(void);
static unsigned int ReadWriteData(unsigned int in_data);
void run_backup(char *filename, unsigned int start, unsigned int length);
void run_erase(char *filename, unsigned int start, unsigned int length);
void run_flash(char *filename, unsigned int start, unsigned int length);
void set_instr(int instr);
void sflash_config(void);
void sflash_erase_area(unsigned int start, unsigned int length);
void sflash_erase_block(unsigned int addr);
void sflash_probe(void);
void sflash_reset(void);
void sflash_write_word(unsigned int addr, unsigned int data);
void show_usage(char *);
void show_fc(void);
void ShowData(unsigned int value);
void test_reset(void);
void WriteData(unsigned int in_data);
void ExecuteDebugModule(unsigned int *pmodule);
void check_ejtag_features(void);


unsigned int pracc_readword_code_module[] = {
               // #
               // # HairyDairyMaid's Assembler PrAcc Read Word Routine
               // #
               // start:
               // 
               // # Load R1 with the address of the pseudo-address register
  0x3C01FF20,  // lui $1,  0xFF20
  0x34210000,  // ori $1,  0x0000
               // 
               // # Load R2 with the address for the read
  0x8C220000,  // lw $2,  ($1)
               // 
               // # Load R3 with the word @R2
  0x8C430000,  // lw $3, 0($2)
               // 
               // # Store the value into the pseudo-data register
  0xAC230004,  // sw $3, 4($1)
               // 
  0x00000000,  // nop
  0x1000FFF9,  // beq $0, $0, start
  0x00000000}; // nop


unsigned int pracc_writeword_code_module[] = {
               // #
               // # HairyDairyMaid's Assembler PrAcc Write Word Routine
               // #
               // start:
               // 
               // # Load R1 with the address of the pseudo-address register
  0x3C01FF20,  // lui $1,  0xFF20
  0x34210000,  // ori $1,  0x0000
               // 
               // # Load R2 with the address for the write
  0x8C220000,  // lw $2,  ($1)
               // 
               // # Load R3 with the data from pseudo-data register
  0x8C230004,  // lw $3, 4($1)
               // 
               // # Store the word at @R2 (the address)
  0xAC430000,  // sw $3,  ($2)
               // 
  0x00000000,  // nop
  0x1000FFF9,  // beq $0, $0, start
  0x00000000}; // nop


unsigned int pracc_readhalf_code_module[] = {
               // #
               // # HairyDairyMaid's Assembler PrAcc Read HalfWord Routine
               // #
               // start:
               // 
               // # Load R1 with the address of the pseudo-address register
  0x3C01FF20,  // lui $1,  0xFF20
  0x34210000,  // ori $1,  0x0000
               // 
               // # Load R2 with the address for the read
  0x8C220000,  // lw $2,  ($1)
               // 
               // # Load R3 with the half word @R2
  0x94430000,  // lhu $3, 0($2)
               // 
               // # Store the value into the pseudo-data register
  0xAC230004,  // sw $3, 4($1)
               // 
  0x00000000,  // nop
  0x1000FFF9,  // beq $0, $0, start
  0x00000000}; // nop


unsigned int pracc_writehalf_code_module[] = {
               // #
               // # HairyDairyMaid's Assembler PrAcc Write HalfWord Routine
               // #
               // start:
               // 
               // # Load R1 with the address of the pseudo-address register
  0x3C01FF20,  // lui $1,  0xFF20
  0x34210000,  // ori $1,  0x0000
               // 
               // # Load R2 with the address for the write
  0x8C220000,  // lw $2,  ($1)
               // 
               // # Load R3 with the data from pseudo-data register
  0x8C230004,  // lw $3, 4($1)
               // 
               // # Store the half word at @R2 (the address)
  0xA4430000,  // sh $3,  ($2)
               // 
  0x00000000,  // nop
  0x1000FFF9,  // beq $0, $0, start
  0x00000000}; // nop


// **************************************************************************
// End of File
// **************************************************************************
