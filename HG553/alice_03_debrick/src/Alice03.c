// **************************************************************************
//
//  Alice03.c - EJTAG Debrick Utility for Alice routers v0.3
//
//  Heavily based on the WRT54G/GS EJTAG Debrick Utility  v4.5/v4.8
//  and on Telsey MAGIC EJTAG Debrick Utility v0.9
//  
//  v 0.1 - Telsey Magic, Pirelli Vela and Pirelli AGA support
//  v 0.2 - Added Pirelli AGB support; all endianness problems corrected
//  v 0.3 - Added Pirelli AGPF support
//
// **************************************************************************
//  Written by 30252783, Janniz, HairyDairyMaid, cattivo12, beghiero 
// **************************************************************************
//
//  Read/write flash memory via EJTAG
//  Usage: Alice03 [parameter] [switches]
//
//              parameter (required)
//              --------------------
//              -backup:<cfe|cfe_nvram|bootimg1|bootimg2|psi_nvram|scratch_pad|psi|wholeflash|custom>
//              -erase:<cfe|cfe_nvram|bootimg1|bootimg2|psi_nvram|scratch_pad|psi|wholeflash|custom>
//              -flash:<cfe|cfe_nvram|bootimg1|bootimg2|psi_nvram|scratch_pad|psi|wholeflash|custom>
//              -probeonly
//              -show:fc
//
//              switches (optional)
//              -------------------
//              /noreset ........... prevent issuing EJTAG CPU reset
//              /noemw ............. prevent enabling memory writes
//              /nocwd ............. prevent clearing CPU watchdog timer
//              /nobreak ........... prevent issuing debug mode JTAGBRK
//              /noerase ........... prevent forced erase before flashing
//              /notimestamp ....... prevent timestamping of backups
//              /dma ............... force use of DMA routines
//              /nodma ............. force use of PRACC routines (no DMA)
//              /window:XXXXXXXX ... custom flash window base (in HEX)
//              /start:XXXXXXXX .... custom start location (in HEX)
//              /length:XXXXXXXX ... custom length (in HEX)
//              /silent ............ prevent scrolling display of data
//              /skipdetect ........ skip auto detection of CPU Chip ID
//              /instrlen:XX ....... set instruction length manually
//              /cable:XXXXXXXX .... set cable type <xilinx|jkeys|wiggler>
//              /fc:XX ............. manual flash chip selection
//
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

// Default is Compile for Linux (both #define's below should be commented out)
#define WINDOWS_VERSION   // uncomment only this for Windows Compile / MS Visual C Compiler
// #define __FreeBSD__       // uncomment only this for FreeBSD

#ifdef WINDOWS_VERSION
   #include <windows.h>      // Only for Windows Compile
   #define strcasecmp  stricmp
   #define strncasecmp strnicmp
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Alice03.h"

static unsigned int ctrl_reg;

int pfd;
int instruction_length;
int issue_reset      = 1;
int issue_enable_mw  = 1;
int issue_watchdog   = 1;
int issue_break      = 1;
int issue_erase      = 1;
int issue_timestamp  = 1;
int force_dma        = 0;
int force_nodma      = 0;
int selected_fc      = 0;
unsigned int selected_window  = 0;
unsigned int selected_start   = 0;
unsigned int selected_length  = 0;
int custom_options   = 0;
int silent_mode      = 0;
int skipdetect       = 0;
int instrlen         = 0;

char            flash_part[128];
unsigned int    flash_size = 0;

int             block_total = 0;
unsigned int    block_addr = 0;
unsigned int    blocks[1024];
unsigned int	id = 0x0;
unsigned int    cmd_type = 0;

char            AREA_NAME[128];
unsigned int    AREA_START;
unsigned int    AREA_LENGTH;
unsigned int    FLASH_MEMORY_START;
unsigned int    vendid;
unsigned int    devid;

unsigned int    data_register;
unsigned int    address_register;

int USE_DMA       = 0;
int ejtag_version = 0;

int selected_cable  = XILINX;

typedef struct _cable_type {
    char	TDI;
    char	TCK;
    char	TMS;
    char	TDO;
    char	TRST_N;
} cable_type;

// TDI, TCK, TMS, TDO
cable_type cable_list[] = {
    {0, 1, 2, 4, -1},		// --- Xilinx Type Cable ---
    {2, 1, 0, 4, -1},		// --- Jkeys Type Cable ---
    {3, 2, 1, 7, 4}			// --- Wiggler Type Cable ---
};

typedef struct _processor_chip_type {
    unsigned int        chip_id;        // Processor Chip ID
    int                 instr_length;   // EJTAG Instruction Length
    char*               chip_descr;     // Processor Chip Description
} processor_chip_type;


processor_chip_type  processor_chip_list[] = {
   // Telsey Magic, Pirelli Vela, Pirelli AGA
   { 0x0634817F, 5, "Broadcom BCM6348 Rev 1 CPU" },
   // Pirelli AGB
   { 0x0633817F, 5, "Broadcom BCM6338 Rev 1 CPU" }, 
   // Pirelli AGPF
   { 0x0635817F, 5, "Broadcom BCM6358 Rev 1 CPU" },
   { 0, 0, 0 }
   };


typedef struct _flash_area_type {
    unsigned int        chip_id;        // Processor Chip ID
    unsigned int        devid;          // Flash Device Id
    char*               area_name;
    unsigned int        area_start;
    unsigned int        area_length;
} flash_area_type;


flash_area_type  flash_area_list[] = {

	// Telsey Magic
	// Pirelli Vela, Pirelli AGA
	// Pirelli AGB
        // Pirelli AGPF

	{ 0x0634817F,	0x22C9,		"WHOLEFLASH",	0x1F800000,		0x800000 },
	{ 0x0634817F,	0x227E,		"WHOLEFLASH",	0x1F000000,		0x800000 },
	{ 0x0633817F,	0x227E,		"WHOLEFLASH",	0x1F800000,		0x800000 },
        { 0x0635817F,   0x227E,         "WHOLEFLASH",   0x1E000000,             0x1000000 },

	{ 0x0634817F,	0x22C9,		"CFE",			0x1FC00000,		0x010000 },
	{ 0x0634817F,	0x227E,		"CFE",			0x1F000000,		0x010000 },
	{ 0x0633817F,	0x227E,		"CFE",			0x1F800000,		0x010000 },
        { 0x0635817F,   0x227E,         "CFE",                  0x1E000000,             0x020000 },

	{ 0x0634817F,	0x22C9,		"NVRAM",		0x1FC00580,		0x000400 },
	{ 0x0634817F,	0x227E,		"NVRAM",		0x1F000580,		0x000400 },
	{ 0x0633817F,	0x227E,		"NVRAM",		0x1F800580,		0x000400 },
        { 0x0635817F,   0x227E,         "NVRAM",                0x1EFF0000,             0x010000 },

	{ 0x0634817F,	0x22C9,		"BOOTIMG1",		0x1FC10000,		0x3E0000 },
	{ 0x0634817F,	0x227E,		"BOOTIMG1",		0x1F010000,		0x3F0000 },
	{ 0x0633817F,	0x227E,		"BOOTIMG1",		0x1F810000,		0x3F0000 },
        { 0x0635817F,   0x227E,         "BOOTIMG1",             0x1E020000,             0x79FF6C },

	{ 0x0634817F,	0x22C9,		"BOOTIMG2",		0x1F800000,		0x400000 },
	{ 0x0634817F,	0x227E,		"BOOTIMG2",		0x1F400000,		0x3F0000 },
	{ 0x0633817F,	0x227E,		"BOOTIMG2",		0x1FC00000,		0x3F0000 },
        { 0x0635817F,   0x227E,         "BOOTIMG2",             0x1E800000,             0x79FF6C },

	{ 0x0634817F,	0x22C9,		"PSI_NVRAM",	0x1FFF0000,		0x008000 },
	{ 0x0634817F,	0x227E,		"PSI_NVRAM",	0x1F7F0000,		0x008000 },
	{ 0x0633817F,	0x227E,		"PSI_NVRAM",	0x1FFF0000,		0x008000 },
	{ 0x0635817F,   0x227E,      	"PSI_NVRAM",   	0x1FFA0000,      	0x01FF6C },

	{ 0x0634817F,	0x22C9,		"SCRATCH_PAD",	0x1FFF8000,		0x002000 },
	{ 0x0634817F,	0x227E,		"SCRATCH_PAD",	0x1F7F8000,		0x002000 },
	{ 0x0633817F,	0x227E,		"SCRATCH_PAD",	0x1FFF8000,		0x002000 },
	{ 0x0635817F,   0x227E,      	"SCRATCH_PAD",  0x1FFC0000,      	0x01FF6C },

	{ 0x0634817F,	0x22C9,		"PSI",			0x1FFFA000,		0x006000 },
	{ 0x0634817F,	0x227E,		"PSI",			0x1F7FA000,		0x006000 },
	{ 0x0633817F,	0x227E,		"PSI",			0x1FFFA000,		0x006000 },
	{ 0x0635817F,   0x227E,      	"PSI",         		0x1FFE0000,      	0x00FF6C },

	{ 0, 0, 0, 0, 0 }
   };


typedef struct _flash_chip_type {
    unsigned int        vendid;         // Manufacturer Id
    unsigned int        devid;          // Device Id
    unsigned int        flash_size;     // Total size in MBytes
    unsigned int        cmd_type;       // Device CMD TYPE
    char*               flash_part;     // Flash Chip Description
    unsigned int        region1_num;    // Region 1 block count
    unsigned int        region1_size;   // Region 1 block size
    unsigned int        region2_num;    // Region 2 block count
    unsigned int        region2_size;   // Region 2 block size
    unsigned int        region3_num;    // Region 3 block count
    unsigned int        region3_size;   // Region 3 block size
    unsigned int        region4_num;    // Region 4 block count
    unsigned int        region4_size;   // Region 4 block size
} flash_chip_type;


flash_chip_type  flash_chip_list[] = {
   // Telsey Magic   
   { 0x00C2, 0x22C9, size8MB, CMD_TYPE_AMD, "MX29LV640T 4Mx16 TopB      (8MB)"   ,127,size64K,  8,size8K,     	0,0,        0,0        },  
   // Pirelli Vela, Pirelli AGA, Pirelli AGB
   { 0x00C2, 0x227E, size8MB, CMD_TYPE_AMD, "MX29LV640MT 4Mx16 TopB     (8MB)"   ,127,size64K,  8,size8K,     	0,0,        0,0        },  
   // Pirelli AGPF
   { 0x0001, 0x227E, size16MB, CMD_TYPE_AMD, "S29gl128p10 16Mx16 TopB    (16MB)" ,128,size128K, 0,0,   		0,0,        0,0        },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
   };


// -----------------------------------------
// ---- Start of Compiler Specific Code ----
// -----------------------------------------


void lpt_openport(void)
{
   #ifdef WINDOWS_VERSION    // ---- Compiler Specific Code ----

      HANDLE h;

      h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
      if(h == INVALID_HANDLE_VALUE) {  printf("Couldn't access giveio device\n");   CloseHandle(h);   exit(0);   }
      CloseHandle(h);

   #else                     // ---- Compiler Specific Code ----

      #ifdef __FreeBSD__     // ---- Compiler Specific Code ----

         pfd = open("/dev/ppi0", O_RDWR);
         if (pfd < 0)   {   perror("Failed to open /dev/ppi0");   exit(0);   }
         if ((ioctl(pfd, PPEXCL) < 0) || (ioctl(pfd, PPCLAIM) < 0))  {   perror("Failed to lock /dev/ppi0");   close(pfd);   exit(0);   }

      #else                  // ---- Compiler Specific Code ----

         pfd = open("/dev/parport0", O_RDWR);
         if (pfd < 0)   {   perror("Failed to open /dev/parport0");   exit(0);   }
         if ((ioctl(pfd, PPEXCL) < 0) || (ioctl(pfd, PPCLAIM) < 0))   {   perror("Failed to lock /dev/parport0");   close(pfd);   exit(0);   }

      #endif

   #endif
}


void lpt_closeport(void)
{
   #ifndef WINDOWS_VERSION   // ---- Compiler Specific Code ----

      #ifndef __FreeBSD__    // ---- Compiler Specific Code ----

         if (ioctl(pfd, PPRELEASE) < 0)  {  perror("Failed to release /dev/parport0");  close(pfd);  exit(0);  }

      #endif

      close(pfd);

   #endif
}


static unsigned char clockin(int tms, int tdi)
{
   unsigned char data;

   tms = tms ? 1 : 0;
   tdi = tdi ? 1 : 0;


   data = (1 << cable_list[selected_cable].TDO) | (0 << cable_list[selected_cable].TCK) | (tms << cable_list[selected_cable].TMS) | (tdi << cable_list[selected_cable].TDI);   
   if (cable_list[selected_cable].TRST_N != -1) 
     data |= (1 << cable_list[selected_cable].TRST_N);
     
    #ifdef WINDOWS_VERSION   // ---- Compiler Specific Code ----
      _outp(0x378, data);
    #else   // ---- Compiler Specific Code ----
      ioctl(pfd, PPWDATA, &data);
    #endif

   data = (1 << cable_list[selected_cable].TDO) | (1 << cable_list[selected_cable].TCK) | (tms << cable_list[selected_cable].TMS) | (tdi << cable_list[selected_cable].TDI);
   if (cable_list[selected_cable].TRST_N != -1) 
     data |= (1 << cable_list[selected_cable].TRST_N);

    #ifdef WINDOWS_VERSION   // ---- Compiler Specific Code ----
      _outp(0x378, data);
      
       data = (unsigned char)_inp(0x379);
    #else   // ---- Compiler Specific Code ----
      ioctl(pfd, PPWDATA, &data);
      
      ioctl(pfd, PPRSTATUS, &data);
    #endif

   data ^= 0x80;
   data >>= cable_list[selected_cable].TDO;
   data &= 1;

   return data;
}


// ---------------------------------------
// ---- End of Compiler Specific Code ----
// ---------------------------------------


void test_reset(void)
{
    clockin(1, 0);  // Run through a handful of clock cycles with TMS high to make sure
    clockin(1, 0);  // we are in the TEST-LOGIC-RESET state.
    clockin(1, 0);
    clockin(1, 0);
    clockin(1, 0);
    clockin(0, 0);  // enter runtest-idle
}


void set_instr(int instr)
{
    int i;
    static int curinstr = 0xFFFFFFFF;

    if (instr == curinstr)
       return;

    clockin(1, 0);  // enter select-dr-scan
    clockin(1, 0);  // enter select-ir-scan
    clockin(0, 0);  // enter capture-ir
    clockin(0, 0);  // enter shift-ir (dummy)
    for (i=0; i < instruction_length; i++)
    {
        clockin(i==(instruction_length - 1), (instr>>i)&1);
    }
    clockin(1, 0);  // enter update-ir
    clockin(0, 0);  // enter runtest-idle

    curinstr = instr;
}


static unsigned int ReadWriteData(unsigned int in_data)
{
    int i;
    unsigned int out_data = 0;
    unsigned char out_bit;

    clockin(1, 0);  // enter select-dr-scan
    clockin(0, 0);  // enter capture-dr
    clockin(0, 0);  // enter shift-dr
    for(i = 0 ; i < 32 ; i++)
    {
       out_bit  = clockin((i == 31), ((in_data >> i) & 1));
       out_data = out_data | (out_bit << i);
    }
    clockin(1,0);   // enter update-dr
    clockin(0,0);   // enter runtest-idle
    return out_data;
}


static unsigned int ReadData(void)
{
    return ReadWriteData(0x00);
}


void WriteData(unsigned int in_data)
{
    ReadWriteData(in_data);
}


void ShowData(unsigned int value)
{
    int i;
    for (i=0; i<32; i++)
        printf("%d", (value >> (31-i)) & 1);
    printf(" (%08X)\n", value);
}


static unsigned int ejtag_read(unsigned int addr)
{
   if (USE_DMA) return(ejtag_dma_read(addr));
   else return(ejtag_pracc_read(addr));
}


static unsigned int ejtag_read_h(unsigned int addr)
{
   if (USE_DMA) return(ejtag_dma_read_h(addr));
   else return(ejtag_pracc_read_h(addr));
}


void ejtag_write(unsigned int addr, unsigned int data)
{
   if (USE_DMA) ejtag_dma_write(addr, data);
   else ejtag_pracc_write(addr, data);
}


void ejtag_write_h(unsigned int addr, unsigned int data)
{
   if (USE_DMA) ejtag_dma_write_h(addr, data);
   else ejtag_pracc_write_h(addr, data);
}


static unsigned int ejtag_dma_read(unsigned int addr)
{
    unsigned int data;
    int retries = RETRY_ATTEMPTS;

begin_ejtag_dma_read:

    // Setup Address
    set_instr(INSTR_ADDRESS);
    WriteData(addr);

    // Initiate DMA Read & set DSTRT
    set_instr(INSTR_CONTROL);
    ctrl_reg = ReadWriteData(DMAACC | DRWN | DMA_WORD | DSTRT | PROBEN | PRACC);

    // Wait for DSTRT to Clear
    while (ReadWriteData(DMAACC | PROBEN | PRACC) & DSTRT);

    // Read Data
    set_instr(INSTR_DATA);
    data = ReadData();

    // Clear DMA & Check DERR
    set_instr(INSTR_CONTROL);
    if (ReadWriteData(PROBEN | PRACC) & DERR)
    {
        if (retries--)  goto begin_ejtag_dma_read;
        else  printf("DMA Read Addr = %08x  Data = (%08x)ERROR ON READ\n", addr, data);
    }

    return(data);
}


static unsigned int ejtag_dma_read_h(unsigned int addr)
{
    unsigned int data;
    int retries = RETRY_ATTEMPTS;

begin_ejtag_dma_read_h:

    // Setup Address
    set_instr(INSTR_ADDRESS);
    WriteData(addr);

    // Initiate DMA Read & set DSTRT
    set_instr(INSTR_CONTROL);
    ctrl_reg = ReadWriteData(DMAACC | DRWN | DMA_HALFWORD | DSTRT | PROBEN | PRACC);

    // Wait for DSTRT to Clear
    while (ReadWriteData(DMAACC | PROBEN | PRACC) & DSTRT);

    // Read Data
    set_instr(INSTR_DATA);
    data = ReadData();

    // Clear DMA & Check DERR
    set_instr(INSTR_CONTROL);
    if (ReadWriteData(PROBEN | PRACC) & DERR)
    {
        if (retries--)  goto begin_ejtag_dma_read_h;
        else  printf("DMA Read Addr = %08x  Data = (%08x)ERROR ON READ\n", addr, data);
    }

    // Handle the bigendian/littleendian
    if ( addr & 0x2 )  data = (data & 0x0000ffff);
    else               data = ((data>>16) & 0xffff);

    return(data);
}


void ejtag_dma_write(unsigned int addr, unsigned int data)
{
    int   retries = RETRY_ATTEMPTS;

begin_ejtag_dma_write:

    // Setup Address
    set_instr(INSTR_ADDRESS);
    WriteData(addr);

    // Setup Data
    set_instr(INSTR_DATA);
    WriteData(data);

    // Initiate DMA Write & set DSTRT
    set_instr(INSTR_CONTROL);
    ctrl_reg = ReadWriteData(DMAACC | DMA_WORD | DSTRT | PROBEN | PRACC);

    // Wait for DSTRT to Clear
    while (ReadWriteData(DMAACC | PROBEN | PRACC) & DSTRT);

    // Clear DMA & Check DERR
    set_instr(INSTR_CONTROL);
    if (ReadWriteData(PROBEN | PRACC) & DERR)
    {
        if (retries--)  goto begin_ejtag_dma_write;
        else  printf("DMA Write Addr = %08x  Data = ERROR ON WRITE\n", addr);
    }
}


void ejtag_dma_write_h(unsigned int addr, unsigned int data)
{
    int   retries = RETRY_ATTEMPTS;

begin_ejtag_dma_write_h:

    // Setup Address
    set_instr(INSTR_ADDRESS);
    WriteData(addr);

    // Setup Data
    set_instr(INSTR_DATA);
    WriteData(data);

    // Initiate DMA Write & set DSTRT
    set_instr(INSTR_CONTROL);
    ctrl_reg = ReadWriteData(DMAACC | DMA_HALFWORD | DSTRT | PROBEN | PRACC);

    // Wait for DSTRT to Clear
    while (ReadWriteData(DMAACC | PROBEN | PRACC) & DSTRT);

    // Clear DMA & Check DERR
    set_instr(INSTR_CONTROL);
    if (ReadWriteData(PROBEN | PRACC) & DERR)
    {
        if (retries--)  goto begin_ejtag_dma_write_h;
        else  printf("DMA Write Addr = %08x  Data = ERROR ON WRITE\n", addr);
    }
}


static unsigned int ejtag_pracc_read(unsigned int addr)
{
   address_register = addr | 0xA0000000;  // Force to use uncached segment
   data_register    = 0x0;
   ExecuteDebugModule(pracc_readword_code_module);
   return(data_register);
}


void ejtag_pracc_write(unsigned int addr, unsigned int data)
{
   address_register = addr | 0xA0000000;  // Force to use uncached segment
   data_register    = data;
   ExecuteDebugModule(pracc_writeword_code_module);
}


static unsigned int ejtag_pracc_read_h(unsigned int addr)
{
   address_register = addr | 0xA0000000;  // Force to use uncached segment
   data_register    = 0x0;
   ExecuteDebugModule(pracc_readhalf_code_module);
   return(data_register);
}


void ejtag_pracc_write_h(unsigned int addr, unsigned int data)
{
   address_register = addr | 0xA0000000;  // Force to use uncached segment
   data_register    = data;
   ExecuteDebugModule(pracc_writehalf_code_module);
}


void ExecuteDebugModule(unsigned int *pmodule)
{
   unsigned int ctrl_reg;
   unsigned int address;
   unsigned int data   = 0;
   unsigned int offset = 0;
   int finished = 0;
   int DEBUGMSG = 0;
      
   if (DEBUGMSG) printf("DEBUGMODULE: Start module.\n");
   
   // Feed the chip an array of 32 bit values into the processor via the EJTAG port as instructions.
   while (1)
   {
      // Read the control register.  Make sure an access is requested, then do it.
      while(1) 
      {
         set_instr(INSTR_CONTROL);
         ctrl_reg = ReadWriteData(PRACC | PROBEN | SETDEV);
         if (ctrl_reg & PRACC)
            break;
         if (DEBUGMSG) printf("DEBUGMODULE: No memory access in progress!\n");
      }
      
      set_instr(INSTR_ADDRESS);
      address = ReadData();
      
      // Check for read or write
      if (ctrl_reg & PRNW) // Bit set for a WRITE
      {
         // Read the data out
         set_instr(INSTR_DATA);
         data = ReadData();
      
         // Clear the access pending bit (let the processor eat!)
         set_instr(INSTR_CONTROL);
         ctrl_reg = ReadWriteData(PROBEN | SETDEV);
      
         // Processor is writing to us
         if (DEBUGMSG) printf("DEBUGMODULE: Write 0x%08X to address 0x%08X\n", data, address);
         // Handle Debug Write
         // If processor is writing to one of our psuedo virtual registers then save off data
         if (address == MIPS_VIRTUAL_ADDRESS_ACCESS)  address_register = data;
         if (address == MIPS_VIRTUAL_DATA_ACCESS)     data_register    = data;
      }
      
      else
      
      {
         // Check to see if its reading at the debug vector.  The first pass through
         // the module is always read at the vector, so the first one we allow.  When
         // the second read from the vector occurs we are done and just exit.
         if (address == MIPS_DEBUG_VECTOR_ADDRESS)
         {
            if (finished++) // Allows ONE pass
            {
               if (DEBUGMSG) printf("DEBUGMODULE: Finished module.\n");
               return;
            }
         }
      
         // Processor is reading from us
         if (address >= MIPS_DEBUG_VECTOR_ADDRESS)
         {
            // Reading an instruction from our module so fetch the instruction from the module
            offset = (address - MIPS_DEBUG_VECTOR_ADDRESS) / 4;
            data = *(unsigned int *)(pmodule + offset);
            if (DEBUGMSG) printf("DEBUGMODULE: Instruction read at 0x%08X  offset -> %04d  data -> 0x%08X\n", address, offset, data); //fflush(stdout);
         }
         else
         {
            // Reading from our virtual register area
            if (DEBUGMSG) printf("DEBUGMODULE: Read address 0x%08X  data = 0x%08X\n", address, data);
            // Handle Debug Read
            // If processor is reading from one of our psuedo virtual registers then give it data
            if (address == MIPS_VIRTUAL_ADDRESS_ACCESS)  data = address_register;
            if (address == MIPS_VIRTUAL_DATA_ACCESS)     data = data_register;
         }
      
         // Send the data out
         set_instr(INSTR_DATA);
         data = ReadWriteData(data);
      
         // Clear the access pending bit (let the processor eat!)
         set_instr(INSTR_CONTROL);
         ctrl_reg = ReadWriteData(PROBEN | SETDEV);
      
      }
   }
}


void chip_detect(void)
{
    
    processor_chip_type*   processor_chip = processor_chip_list;

    lpt_openport();

    printf("Probing bus ... ");
    
    if (skipdetect)
    {
    	 // Manual Override CPU Chip ID
       test_reset();
       instruction_length = instrlen;
       set_instr(INSTR_IDCODE);
       id = ReadData();
       printf("Done\n\n");
       printf("Instruction Length set to %d\n\n",instruction_length);
       printf("CPU Chip ID: ");  ShowData(id);  printf("*** CHIP DETECTION OVERRIDDEN ***\n\n");
       return;
    }
    else
    {
    	 // Auto Detect CPU Chip ID
       while (processor_chip->chip_id)
       {
          test_reset();
          if (instrlen) 
             instruction_length = instrlen;
          else 
             instruction_length = processor_chip->instr_length;
          set_instr(INSTR_IDCODE);
          id = ReadData();
          if (id == processor_chip->chip_id)
          {
             printf("Done\n\n");
             printf("Instruction Length set to %d\n\n",instruction_length);
             printf("CPU Chip ID: ");  ShowData(id);  printf("*** Found a %s chip ***\n\n", processor_chip->chip_descr);
             return;
          }
          processor_chip++;
       }
    }

    printf("Done\n\n");
    printf("Instruction Length set to %d\n\n",instruction_length);
    printf("CPU Chip ID: ");  ShowData(id);  printf("*** Unknown or NO CPU Chip ID Detected ***\n\n");
    
    printf("*** Possible Causes:\n");
    printf("    1) Alice router is not Connected.\n");
    printf("    2) Alice router is not Powered On.\n");
    printf("    3) Improper JTAG Cable.\n");
    printf("    4) Unrecognized CPU Chip ID.\n");

    chip_shutdown();;
    exit(0);
}


void check_ejtag_features() 
{
    unsigned int features;
    
    set_instr(INSTR_IMPCODE);
    features = ReadData();

    printf("    - EJTAG IMPCODE ....... : ");   ShowData(features);
  
    // EJTAG Version 
    ejtag_version = (features >> 29) & 7;
    printf("    - EJTAG Version ....... : ");
    if (ejtag_version == 0)       printf("1 or 2.0\n");
    else if (ejtag_version == 1)  printf("2.5\n");
    else if (ejtag_version == 2)  printf("2.6\n");
    else                          printf("Unknown (%d is a reserved value)\n", ejtag_version);

    // EJTAG DMA Support
    USE_DMA = !(features & (1 << 14));
    printf("    - EJTAG DMA Support ... : %s\n", USE_DMA ? "Yes" : "No");

    if (force_dma)   { USE_DMA = 1;  printf("    *** DMA Mode Forced On ***\n"); }
    if (force_nodma) { USE_DMA = 0;  printf("    *** DMA Mode Forced Off ***\n"); }
        
    printf("\n");
}


void chip_shutdown(void)
{
    fflush(stdout);
    test_reset();
    lpt_closeport();
}


void run_backup(char *filename, unsigned int start, unsigned int length)
{
    unsigned int addr, data;
    FILE *fd;
    int counter = 0;
    int percent_complete = 0;
    char newfilename[128] = "";
    time_t start_time = time(0);
    time_t end_time, elapsed_seconds;
    struct tm* lt = localtime(&start_time);
    char time_str[15];
	unsigned char buffer[4];
    
    sprintf(time_str, "%04d%02d%02d_%02d%02d%02d",
        lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
        lt->tm_hour, lt->tm_min, lt->tm_sec
    );

    printf("*** You Selected to Backup the %s ***\n\n",filename);

    strcpy(newfilename,filename);
    strcat(newfilename,".SAVED");
    if (issue_timestamp)
    {
       strcat(newfilename,"_");
       strcat(newfilename,time_str);
    }

    fd = fopen(newfilename, "wb" );
    if (fd<=0)
    {
        fprintf(stderr,"Could not open %s for writing\n", newfilename);
        exit(1);
    }

    printf("=========================\n");
    printf("Backup Routine Started\n");
    printf("=========================\n");

    printf("\nSaving %s to Disk...\n",newfilename);
    for(addr=start; addr<(start+length); addr+=4)
    {
        counter += 4;
        percent_complete = (counter * 100 / length);
        if (!silent_mode)
           if ((addr&0xF) == 0)  printf("[%3d%% Backed Up]   %08x: ", percent_complete, addr);
         
       data = ejtag_read(addr);
	   buffer[0]= (unsigned char) (data >> 24);
	   buffer[1]= (unsigned char) (data >> 16);
	   buffer[2]= (unsigned char) (data >> 8);
	   buffer[3]= (unsigned char) (data);

	   fwrite(buffer, sizeof(buffer[0]), sizeof(buffer), fd);
//	   fwrite( (unsigned char*) &data, 1, sizeof(data), fd);
          
       if (silent_mode)  printf("%4d%%   bytes = %d\r", percent_complete, counter);
       else              printf("%08x%c", data, (addr&0xF)==0xC?'\n':' ');
       
       fflush(stdout); 
      }
    fclose(fd);

    printf("Done  (%s saved to Disk OK)\n\n",newfilename);

    printf("bytes written: %d\n", counter);
    
    printf("=========================\n");
    printf("Backup Routine Complete\n");
    printf("=========================\n");

    time(&end_time);
    elapsed_seconds = difftime(end_time, start_time); 
    printf("elapsed time: %d seconds\n", (int)elapsed_seconds);
}



void run_flash(char *filename, unsigned int start, unsigned int length)
{
    unsigned int addr, data ;
    FILE *fd ;
    int counter = 0;
    int percent_complete = 0;
    time_t start_time = time(0);
    time_t end_time, elapsed_seconds;
	unsigned char buffer[4];

    printf("*** You Selected to Flash the %s ***\n\n",filename);

    fd=fopen(filename, "rb" );
    if (fd<=0)
    {
        fprintf(stderr,"Could not open %s for reading\n", filename);
        exit(1);
    }

    printf("=========================\n");
    printf("Flashing Routine Started\n");
    printf("=========================\n");

    if (issue_erase) sflash_erase_area(start,length);

    printf("\nLoading %s to Flash Memory...\n",filename);

    
    for(addr=start; addr<(start+length); addr+=4)
    {
        counter += 4;
	
        percent_complete = (counter * 100 / length);
        if (!silent_mode)
           if ((addr&0xF) == 0)  printf("[%3d%% Flashed]   %08x: ", percent_complete, addr);

		fread(buffer, sizeof(buffer[0]), sizeof(buffer), fd);
		data = ((unsigned int) buffer[0]) << 24;
		data |= ((unsigned int) buffer[1]) << 16;
		data |= ((unsigned int) buffer[2]) << 8;
		data |= ((unsigned int) buffer[3]);
//		fread( (unsigned char*) &data, 1,sizeof(data), fd);
	
        // Erasing Flash Sets addresses to 0xFF's so we can avoid writing these (for speed)
        if (issue_erase) {
           if (!(data == 0xFFFFFFFF))
              sflash_write_word(addr, data);
        }
        else sflash_write_word(addr, data);  // Otherwise we gotta flash it all

        if (silent_mode)  printf("%4d%%   bytes = %d\r", percent_complete, counter);
        else              printf("%08x%c", data, (addr&0xF)==0xC?'\n':' ');
        
        fflush(stdout);
//        data = 0xFFFFFFFF;
		buffer[0] = buffer[1] = buffer[2] = buffer[3] = 0xFF;  // This is in case file is shorter than expected length
      }
    fclose(fd);
    printf("Done  (%s loaded into Flash Memory OK)\n\n",filename);

    sflash_reset();

    printf("=========================\n");
    printf("Flashing Routine Complete\n");
    printf("=========================\n");

    time(&end_time);
    elapsed_seconds = difftime(end_time, start_time); 
    printf("elapsed time: %d seconds\n", (int)elapsed_seconds);
}


void run_erase(char *filename, unsigned int start, unsigned int length)
{
    time_t start_time = time(0);
    time_t end_time, elapsed_seconds;

    printf("*** You Selected to Erase the %s ***\n\n",filename);

    printf("=========================\n");
    printf("Erasing Routine Started\n");
    printf("=========================\n");

    sflash_erase_area(start,length);
    sflash_reset();

    printf("=========================\n");
    printf("Erasing Routine Complete\n");
    printf("=========================\n");

    time(&end_time);
    elapsed_seconds = difftime(end_time, start_time); 
    printf("elapsed time: %d seconds\n", (int)elapsed_seconds);
}


void identify_flash_part(void)
{
   flash_chip_type*   flash_chip = flash_chip_list;
   flash_area_type*   flash_area = flash_area_list;

   // Important for these to initialize to zero
   block_addr	= 0;
   block_total	= 0;
   flash_size	= 0;
   cmd_type		= 0;
   strcpy(flash_part,"");

   while (flash_chip->vendid)
   {
      if ((flash_chip->vendid == vendid) && (flash_chip->devid == devid))
      {
         flash_size = flash_chip->flash_size;
         cmd_type   = flash_chip->cmd_type;
         strcpy(flash_part, flash_chip->flash_part);
	 
         while (flash_area->chip_id)
         {
            if ((flash_area->chip_id == id) && (flash_area->devid == devid) && (strcasecmp(flash_area->area_name, "WHOLEFLASH")==0))
               FLASH_MEMORY_START = flash_area->area_start;
            if ((flash_area->chip_id == id) && (flash_area->devid == devid) && (strcasecmp(flash_area->area_name, AREA_NAME)==0))
            {
               strcat(AREA_NAME,".BIN");
               AREA_START  = flash_area->area_start;
               AREA_LENGTH = flash_area->area_length;
               break;
            }
            flash_area++;
         }
	 	          
         if (strcasecmp(AREA_NAME,"CUSTOM")==0)
         {
            strcat(AREA_NAME,".BIN");
            FLASH_MEMORY_START = selected_window;
            AREA_START         = selected_start;
            AREA_LENGTH        = selected_length;
         }

         if (flash_chip->region1_num)  define_block(flash_chip->region1_num, flash_chip->region1_size);
         if (flash_chip->region2_num)  define_block(flash_chip->region2_num, flash_chip->region2_size);
         if (flash_chip->region3_num)  define_block(flash_chip->region3_num, flash_chip->region3_size);
         if (flash_chip->region4_num)  define_block(flash_chip->region4_num, flash_chip->region4_size);

         sflash_reset();

         printf("Done\n\n");
         printf("Flash Vendor ID: ");  ShowData(vendid);
         printf("Flash Device ID: ");  ShowData(devid);
         if (selected_fc != 0)
            printf("*** Manually Selected a %s Flash Chip ***\n\n", flash_part);
         else
            printf("*** Found a %s Flash Chip ***\n\n", flash_part);

         printf("    - Flash Chip Window Start .... : %08x\n", FLASH_MEMORY_START);
         printf("    - Flash Chip Window Length ... : %08x\n", flash_size);
         printf("    - Selected Area Start ........ : %08x\n", AREA_START);
         printf("    - Selected Area Length ....... : %08x\n\n", AREA_LENGTH);
         
         break;
      }
      flash_chip++;
   }
}


void define_block(unsigned int block_count, unsigned int block_size)
{
  unsigned int  i;

  if (block_addr == 0)  block_addr = FLASH_MEMORY_START;

  for (i = 1; i <= block_count; i++)
  {
     block_total++;
     blocks[block_total] = block_addr;
     block_addr = block_addr + block_size;
  }
}


void sflash_config(void)
{
   flash_chip_type*   flash_chip = flash_chip_list;
   int counter = 0;

   printf("\nManual Flash Selection ... ");

   while (flash_chip->vendid)
   {
      counter++;
      if (counter == selected_fc)
      {
         vendid = flash_chip->vendid;
         devid  = flash_chip->devid;
         identify_flash_part();
         break;
      }
      flash_chip++;
   }

    if (strcasecmp(flash_part,"")==0)
       printf("*** Unknown or NO Flash Chip Selected ***\n");

}


void sflash_probe(void)
{
   int retries = 200;
   int output = 0;

    // Default to Standard Flash Window for Detection if not CUSTOM
    if (strcasecmp(AREA_NAME,"CUSTOM")==0)
         FLASH_MEMORY_START = selected_window;
    else FLASH_MEMORY_START = 0x1F000000;
//    else FLASH_MEMORY_START = 0x1F800000;
//    else FLASH_MEMORY_START = 0x1FC00000;

    printf("\nProbing Flash at (Flash Window: 0x%08x) ... ", FLASH_MEMORY_START);

again:

	strcpy(flash_part,"");

    // Probe using cmd_type for AMD
    if (strcasecmp(flash_part,"")==0)
    {
       cmd_type = CMD_TYPE_AMD;
       sflash_reset();
       ejtag_write_h(FLASH_MEMORY_START + (0x555 << 1), 0x00AA00AA);
       ejtag_write_h(FLASH_MEMORY_START + (0x2AA << 1), 0x00550055);
       ejtag_write_h(FLASH_MEMORY_START + (0x555 << 1), 0x00900090);
       vendid = ejtag_read_h(FLASH_MEMORY_START);
       devid  = ejtag_read_h(FLASH_MEMORY_START+2);
       identify_flash_part();
	   if (strcasecmp(flash_part,"")==0 && vendid!=0 && output==0)
	   {
		   printf("\nProbe using cmd_type for AMD");
		   printf("\nFlash Vendor ID: %08X", vendid);
		   printf("\nFlash Device ID: %08X\n", devid);
		   output++;
	   }
    }

    // Probe using cmd_type for SST
    if (strcasecmp(flash_part,"")==0)
    {
       cmd_type = CMD_TYPE_SST;
       sflash_reset();
       ejtag_write_h(FLASH_MEMORY_START + (0x5555 << 1), 0x00AA00AA);
       ejtag_write_h(FLASH_MEMORY_START + (0x2AAA << 1), 0x00550055);
       ejtag_write_h(FLASH_MEMORY_START + (0x5555 << 1), 0x00900090);
       vendid = ejtag_read_h(FLASH_MEMORY_START);
       devid  = ejtag_read_h(FLASH_MEMORY_START+2);
       identify_flash_part();
	   if (strcasecmp(flash_part,"")==0 && vendid!=0 && output==0)
	   {
		   printf("\nProbe using cmd_type for SST");
		   printf("\nFlash Vendor ID: %08X", vendid);
		   printf("\nFlash Device ID: %08X\n", devid);
		   output++;
	   }
    }

    // Probe using cmd_type for BSC & SCS
    if (strcasecmp(flash_part,"")==0)
    {
       cmd_type = CMD_TYPE_BSC;
       sflash_reset();
       ejtag_write_h(FLASH_MEMORY_START, 0x00900090);
       vendid = ejtag_read_h(FLASH_MEMORY_START);
       devid  = ejtag_read_h(FLASH_MEMORY_START+2);
       identify_flash_part();
	   if (strcasecmp(flash_part,"")==0 && vendid!=0 && output==0)
	   {
		   printf("\nProbe using cmd_type for BSC & SCS");
		   printf("\nFlash Vendor ID: %08X", vendid);
		   printf("\nFlash Device ID: %08X\n", devid);
		   output++;
	   }
    }

    if (strcasecmp(flash_part,"")==0)
    {
       if (retries--)
	   {
		   if (retries % 50 == 0)
		   {
			   FLASH_MEMORY_START += 0x400000;
			   printf("\nProbing Flash at (Flash Window: 0x%08x) ... ", FLASH_MEMORY_START);
		   }
		   goto again;
	   }
       else
       {
       	  printf("Done\n\n");
          printf("*** Unknown or NO Flash Chip Detected ***");
       }
    }

    return;
}

void sflash_poll(unsigned int addr, unsigned int data)
{

 if ((cmd_type == CMD_TYPE_BSC) || (cmd_type == CMD_TYPE_SCS))
 {
    // Wait Until Ready
    while ( (ejtag_read_h(FLASH_MEMORY_START) & STATUS_READY) != STATUS_READY );
 }
 else
 {
    // Wait Until Ready
    while ( (ejtag_read_h(addr) & STATUS_READY) != (data & STATUS_READY) );
 }
 
}

void sflash_erase_area(unsigned int start, unsigned int length)
{
    int cur_block;
    int tot_blocks;
    unsigned int reg_start;
    unsigned int reg_end;


    reg_start = start;
    reg_end   = reg_start + length;

    tot_blocks = 0;

    for (cur_block = 1;  cur_block <= block_total;  cur_block++)
    {
       block_addr = blocks[cur_block];
       if ((block_addr >= reg_start) && (block_addr < reg_end))  tot_blocks++;
    }

    printf("Total Blocks to Erase: %d\n\n", tot_blocks);

    for (cur_block = 1;  cur_block <= block_total;  cur_block++)
    {
       block_addr = blocks[cur_block];
       if ((block_addr >= reg_start) && (block_addr < reg_end))
          {
             printf("Erasing block: %d (addr = %08x)...", cur_block, block_addr);  fflush(stdout);
             sflash_erase_block(block_addr);
             printf("Done\n\n");  fflush(stdout);
          }
    }

}


void sflash_erase_block(unsigned int addr)
{

    if (cmd_type == CMD_TYPE_AMD)
    {

        //Unlock Block
        ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00AA00AA);
        ejtag_write_h(FLASH_MEMORY_START+(0x2AA << 1), 0x00550055);
        ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00800080);

        //Erase Block
        ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00AA00AA);
        ejtag_write_h(FLASH_MEMORY_START+(0x2AA << 1), 0x00550055);
        ejtag_write_h(addr, 0x00300030);

        // Wait for Erase Completion
        sflash_poll(addr, 0xFFFF);
    }

    if (cmd_type == CMD_TYPE_SST)
    {

        //Unlock Block
        ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00AA00AA);
        ejtag_write_h(FLASH_MEMORY_START+(0x2AAA << 1), 0x00550055);
        ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00800080);

        //Erase Block
        ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00AA00AA);
        ejtag_write_h(FLASH_MEMORY_START+(0x2AAA << 1), 0x00550055);
        ejtag_write_h(addr, 0x00500050);

        // Wait for Erase Completion
        sflash_poll(addr, 0xFFFF);
    }

    if ((cmd_type == CMD_TYPE_BSC) || (cmd_type == CMD_TYPE_SCS))
    {

        //Unlock Block
        ejtag_write_h(addr, 0x00500050);     // Clear Status Command	
        ejtag_write_h(addr, 0x00600060);     // Unlock Flash Block Command
        ejtag_write_h(addr, 0x00D000D0);     // Confirm Command

        // Wait for Unlock Completion
        sflash_poll(addr, STATUS_READY);

        //Erase Block
        ejtag_write_h(addr, 0x00500050);     // Clear Status Command
        ejtag_write_h(addr, 0x00200020);     // Block Erase Command
        ejtag_write_h(addr, 0x00D000D0);     // Confirm Command

        // Wait for Erase Completion
        sflash_poll(addr, 0xFFFF);
    }

    sflash_reset();

}


void sflash_reset(void)
{

    if ((cmd_type == CMD_TYPE_AMD) || (cmd_type == CMD_TYPE_SST))
    {
        ejtag_write_h(FLASH_MEMORY_START, 0x00F000F0);    // Set array to read mode
    }

    if ((cmd_type == CMD_TYPE_BSC) || (cmd_type == CMD_TYPE_SCS))
    {
        ejtag_write_h(FLASH_MEMORY_START, 0x00500050);    // Clear CSR
        ejtag_write_h(FLASH_MEMORY_START, 0x00ff00ff);    // Set array to read mode
    }

}


void sflash_write_word(unsigned int addr, unsigned int data)
{
unsigned int data_lo, data_hi;
unsigned int half1, half2;

    if (USE_DMA)
    {
		// DMA Uses Byte Lanes
		data_lo = data;
		data_hi = data;

		half1 = ((data >> 16) & 0xFFFF);
		half2 = (data & 0xFFFF);
             
    }
    else   
    {
		// PrAcc Does Not
		data_lo = ((data >> 16) & 0xFFFF);
		data_hi = (data & 0xFFFF);
       
		half1 = data_lo;
		half2 = data_hi;
    }

    if (cmd_type == CMD_TYPE_AMD)
    {

      // Handle Half Of Word
      ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00AA00AA);
      ejtag_write_h(FLASH_MEMORY_START+(0x2AA << 1), 0x00550055);
      ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00A000A0);
      ejtag_write_h(addr, data_lo);

       // Wait for Completion
      sflash_poll(addr, half1);

      // Now Handle Other Half Of Word
      ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00AA00AA);
      ejtag_write_h(FLASH_MEMORY_START+(0x2AA << 1), 0x00550055);
      ejtag_write_h(FLASH_MEMORY_START+(0x555 << 1), 0x00A000A0);
      
      ejtag_write_h(addr+2, data_hi);

      // Wait for Completion
      sflash_poll(addr+2, half2); 
    }

    if (cmd_type == CMD_TYPE_SST)
    {
      // Handle Half Of Word
      ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00AA00AA);
      ejtag_write_h(FLASH_MEMORY_START+(0x2AAA << 1), 0x00550055);
      ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00A000A0);
      ejtag_write_h(addr, data_lo);

       // Wait for Completion
       sflash_poll(addr, (data & 0xffff));

      // Now Handle Other Half Of Word
      ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00AA00AA);
      ejtag_write_h(FLASH_MEMORY_START+(0x2AAA << 1), 0x00550055);
      ejtag_write_h(FLASH_MEMORY_START+(0x5555 << 1), 0x00A000A0);
      ejtag_write_h(addr+2, data_hi);

      // Wait for Completion
      sflash_poll(addr+2, ((data >> 16) & 0xffff));
    }

    if ((cmd_type == CMD_TYPE_BSC) || (cmd_type == CMD_TYPE_SCS))
    {
       // Handle Half Of Word
       ejtag_write_h(addr, 0x00500050);     // Clear Status Command
       ejtag_write_h(addr, 0x00400040);     // Write Command
       ejtag_write_h(addr, data_lo);        // Send HalfWord Data
       ejtag_write_h(addr, 0x00700070);     // Check Status Command

       // Wait for Completion
       sflash_poll(addr, STATUS_READY);
       
       // Now Handle Other Half Of Word
       ejtag_write_h(addr+2, 0x00500050);   // Clear Status Command
       ejtag_write_h(addr+2, 0x00400040);   // Write Command
       ejtag_write_h(addr+2, data_hi);      // Send HalfWord Data
       ejtag_write_h(addr+2, 0x00700070);   // Check Status Command

       // Wait for Completion
       sflash_poll(addr+2, STATUS_READY);
    }
}


void show_usage(char *myName)
{

//   flash_chip_type*      flash_chip = flash_chip_list;
//   processor_chip_type*  processor_chip = processor_chip_list;
//   int counter = 0;

   printf( " ABOUT: This program is a debrick utility for Alice routers.\n"
           "        It's able to read/write flash memory on the routers distributed\n"
           "        by Telecom Italia via EJTAG using either DMA Access routines\n"
           "        or PrAcc routines (slower/more compatible). \n");
   printf( "\n\n");
   printf( " USAGE: %s PARAMETER [option]...\n"
		"\n"
			"            Required Parameter\n"
			"            ------------------\n"
			"            -backup:<cfe|cfe_nvram|bootimg1|bootimg2|psi_nvram|scratch_pad|psi|wholeflash|custom>\n"
			"            -erase:<cfe|cfe_nvram|bootimg1|bootimg2|psi_nvram|scratch_pad|psi|wholeflash|custom>\n"
			"            -flash:<cfe|cfe_nvram|bootimg1|bootimg2|psi_nvram|scratch_pad|psi|wholeflash|custom>\n"
			"            -probeonly\n"
			"            -show:fc\n"
		"\n"	   
			"            Optional Switches\n"
			"            -----------------\n"
			"            /noreset ........... prevent Issuing EJTAG CPU reset\n"
			"            /noemw ............. prevent Enabling Memory Writes\n"
			"            /nocwd ............. prevent Clearing CPU Watchdog Timer\n"
			"            /nobreak ........... prevent Issuing Debug Mode JTAGBRK\n"
			"            /noerase ........... prevent Forced Erase before Flashing\n"
			"            /notimestamp ....... prevent Timestamping of Backups\n"
			"            /dma ............... force use of DMA routines\n"
			"            /nodma ............. force use of PRACC routines (No DMA)\n"
			"            /window:XXXXXXXX ... custom flash window base (in HEX)\n"
			"            /start:XXXXXXXX .... custom start location (in HEX)\n"
			"            /length:XXXXXXXX ... custom length (in HEX)\n"
			"            /silent ............ prevent scrolling display of data\n"
			"            /skipdetect ........ skip auto detection of CPU Chip ID\n"
			"            /instrlen:XX ....... set instruction length manually\n"
			"            /cable:XXXXXXXX .... set cable type (xilinx, jkeys, wiggler)\n"
		"\n"
			"            /fc:XX = manual Flash Chip selection\n"
			"                     Use the parameter -show:/fc to see the\n"
			"                     complete chip list\n"	   
			"            -----------------------------------------------\n", myName);
   printf( "\n\n");
   printf( " NOTES: 1) If 'flashing' - the source filename must exist as follows:\n"
           "           CFE.BIN, NVRAM.BIN, BOOTIMG<1|2>.BIN, PSI_NVRAM.BIN,\n"
           "           SCRATCH_PAD.BIN, PSI.BIN, WHOLEFLASH.BIN or CUSTOM.BIN\n\n"
           
           "        2) You have to make a power cicle before any operation.\n"
           
           " ******************************************************************************\n"
           " * Flashing the BOOTIMGes or WHOLEFLASH will take a very long time using JTAG *\n"
           " * via this utility.  You are better off flashing the CFE & NVRAM files       *\n"
           " * and then using the normal CFE, TFTP or Web method to flash the BOOTIMGes   *\n"
           " * via ethernet.                                                              *\n"
           " ******************************************************************************\n\n");
}

void show_fc()
{
   flash_chip_type*      flash_chip = flash_chip_list;
   int counter = 0;

   printf("Manual Flash Chip Selection.\n\n"
          "Supported flash list: \n\n");

   while (flash_chip->vendid)
   {
      printf("            /fc:%02d ............. %-40.40s\n", ++counter, flash_chip->flash_part);
      flash_chip++;
   }
   
   printf( "\n\n");
}

char decode_cable(char* cableString)
{
    if (strcasecmp(cableString,"xilinx")==0)		return XILINX;
    else if (strcasecmp(cableString,"jkeys")==0)	return JKEYS;
    else if (strcasecmp(cableString,"wiggler")==0)	return WIGGLER;
    
    return -1;
}

int main(int argc, char** argv)
{
    char choice[128];
    int run_option;
    int j;

    printf("\n");
    printf("============================================\n");
    printf("EJTAG Debrick Utility for Alice routers v0.3\n");
    printf("============================================\n\n");

    if (argc < 2)
    {
        show_usage(argv[0]);
        exit(1);
    }
    
    strcpy(choice,argv[1]);

    run_option = 0;

    if (strcasecmp(choice,"-backup:cfe")==0)         { run_option = 1;  strcpy(AREA_NAME, "CFE");         }
    if (strcasecmp(choice,"-backup:cfe_nvram")==0)   { run_option = 1;  strcpy(AREA_NAME, "NVRAM");       }
    if (strcasecmp(choice,"-backup:bootimg1")==0)    { run_option = 1;  strcpy(AREA_NAME, "BOOTIMG1");    }
    if (strcasecmp(choice,"-backup:bootimg2")==0)    { run_option = 1;  strcpy(AREA_NAME, "BOOTIMG2");    }    
    if (strcasecmp(choice,"-backup:psi_nvram")==0)   { run_option = 1;  strcpy(AREA_NAME, "PSI_NVRAM");   }
    if (strcasecmp(choice,"-backup:scratch_pad")==0) { run_option = 1;  strcpy(AREA_NAME, "SCRATCH_PAD"); }
    if (strcasecmp(choice,"-backup:psi")==0)         { run_option = 1;  strcpy(AREA_NAME, "PSI");         }    
    if (strcasecmp(choice,"-backup:wholeflash")==0)  { run_option = 1;  strcpy(AREA_NAME, "WHOLEFLASH");  }
    if (strcasecmp(choice,"-backup:custom")==0)      { run_option = 1;  strcpy(AREA_NAME, "CUSTOM");  custom_options++; }

    if (strcasecmp(choice,"-erase:cfe")==0)          { run_option = 2;  strcpy(AREA_NAME, "CFE");         }
    if (strcasecmp(choice,"-erase:cfe_nvram")==0)    { run_option = 2;  strcpy(AREA_NAME, "NVRAM");       }
    if (strcasecmp(choice,"-erase:bootimg1")==0)     { run_option = 2;  strcpy(AREA_NAME, "BOOTIMG1");    }
    if (strcasecmp(choice,"-erase:bootimg2")==0)     { run_option = 2;  strcpy(AREA_NAME, "BOOTIMG2");    }    
    if (strcasecmp(choice,"-erase:psi_nvram")==0)    { run_option = 2;  strcpy(AREA_NAME, "PSI_NVRAM");   }
    if (strcasecmp(choice,"-erase:scratch_pad")==0)  { run_option = 2;  strcpy(AREA_NAME, "SCRATCH_PAD"); }
    if (strcasecmp(choice,"-erase:psi")==0)          { run_option = 2;  strcpy(AREA_NAME, "PSI");         }        
    if (strcasecmp(choice,"-erase:wholeflash")==0)   { run_option = 2;  strcpy(AREA_NAME, "WHOLEFLASH");  }
    if (strcasecmp(choice,"-erase:custom")==0)       { run_option = 2;  strcpy(AREA_NAME, "CUSTOM");  custom_options++; }

    if (strcasecmp(choice,"-flash:cfe")==0)          { run_option = 3;  strcpy(AREA_NAME, "CFE");         }
    if (strcasecmp(choice,"-flash:cfe_nvram")==0)    { run_option = 3;  strcpy(AREA_NAME, "NVRAM");       }
    if (strcasecmp(choice,"-flash:bootimg1")==0)     { run_option = 3;  strcpy(AREA_NAME, "BOOTIMG1");    }
    if (strcasecmp(choice,"-flash:bootimg2")==0)     { run_option = 3;  strcpy(AREA_NAME, "BOOTIMG2");    }
    if (strcasecmp(choice,"-flash:psi_nvram")==0)    { run_option = 3;  strcpy(AREA_NAME, "PSI_NVRAM");   }
    if (strcasecmp(choice,"-flash:scratch_pad")==0)  { run_option = 3;  strcpy(AREA_NAME, "SCRATCH_PAD"); }
    if (strcasecmp(choice,"-flash:psi")==0)          { run_option = 3;  strcpy(AREA_NAME, "PSI");         }            
    if (strcasecmp(choice,"-flash:wholeflash")==0)   { run_option = 3;  strcpy(AREA_NAME, "WHOLEFLASH");  }
    if (strcasecmp(choice,"-flash:custom")==0)       { run_option = 3;  strcpy(AREA_NAME, "CUSTOM");  custom_options++; }

    if (strcasecmp(choice,"-probeonly")==0)          { run_option = 4;  }

    if (strcasecmp(choice,"-show:fc")==0)            { show_fc(); exit(0); }

    if (run_option == 0)
    {
        show_usage(argv[0]);
        printf("\n*** ERROR - Invalid [option] specified ***\n\n");
        exit(1);
    }

    if (argc > 2)
    {
       j = 2;
       while (j < argc)
       {
          strcpy(choice,argv[j]);

          if (strcasecmp(choice,"/noreset")==0)              issue_reset = 0;
          else if (strcasecmp(choice,"/noemw")==0)           issue_enable_mw = 0;
          else if (strcasecmp(choice,"/nocwd")==0)           issue_watchdog = 0;
          else if (strcasecmp(choice,"/nobreak")==0)         issue_break = 0;
          else if (strcasecmp(choice,"/noerase")==0)         issue_erase = 0;
          else if (strcasecmp(choice,"/notimestamp")==0)     issue_timestamp = 0;
          else if (strcasecmp(choice,"/dma")==0)             force_dma = 1;
          else if (strcasecmp(choice,"/nodma")==0)           force_nodma = 1;
          else if (strncasecmp(choice,"/fc:",4)==0)          selected_fc = strtoul(((char *)choice + 4),NULL,10);
          else if (strncasecmp(choice,"/window:",8)==0)    { selected_window = strtoul(((char *)choice + 8),NULL,16); custom_options++;  }
          else if (strncasecmp(choice,"/start:",7)==0)     { selected_start  = strtoul(((char *)choice + 7),NULL,16); custom_options++;  }
          else if (strncasecmp(choice,"/length:",8)==0)    { selected_length = strtoul(((char *)choice + 8),NULL,16); custom_options++;  }
          else if (strcasecmp(choice,"/silent")==0)          silent_mode = 1;
          else if (strcasecmp(choice,"/skipdetect")==0)      skipdetect = 1;
          else if (strncasecmp(choice,"/instrlen:",10)==0)   instrlen = strtoul(((char *)choice + 10),NULL,10);
	  else if (strncasecmp(choice,"/cable:",7)==0 && \
		   ((selected_cable = decode_cable((char *)choice + 7))!=-1));	  
          else
          {
             show_usage(argv[0]);	  
             printf("\n*** ERROR - Invalid <option> specified ***\n\n");
             exit(1);
          }
          j++;
       }
    }
    
    if (strcasecmp(AREA_NAME,"CUSTOM")==0)
    {
       if ((custom_options != 0) && (custom_options != 4))
       {
          show_usage(argv[0]);
          printf("\n*** ERROR - 'CUSTOM' also requires '/window' '/start' and '/length' options ***\n\n");
          exit(1);
       }
    }


    // ----------------------------------
    // Detect CPU 
    // ----------------------------------
    chip_detect();


    // ----------------------------------
    // Find Implemented EJTAG Features
    // ----------------------------------
    check_ejtag_features();


    // ----------------------------------
    // Reset State Machine For Good Measure
    // ----------------------------------
    test_reset();


    // ----------------------------------
    // Reset Processor and Peripherals
    // ----------------------------------
    printf("Issuing Processor / Peripheral Reset ... ");
    if (issue_reset)
    {
       set_instr(INSTR_CONTROL);
       ctrl_reg = ReadWriteData(PRRST | PERRST);
       printf("Done\n");
    } else printf("Skipped\n");


    // ----------------------------------
    // Enable Memory Writes
    // ----------------------------------
    // Always skip for EJTAG versions 2.5 and 2.6 since they do not support DMA transactions.
    // Memory Protection bit only applies to EJTAG 2.0 based chips.
    if (ejtag_version != 0)  issue_enable_mw = 0;
    printf("Enabling Memory Writes ... ");   
    if (issue_enable_mw)
    {
    	 // Clear Memory Protection Bit in DCR
       ejtag_dma_write(0xff300000, (ejtag_dma_read(0xff300000) & ~(1<<2)) );
       printf("Done\n");
    } else printf("Skipped\n");


    // ----------------------------------
    // Put into EJTAG Debug Mode
    // ----------------------------------
    printf("Halting Processor ... ");
    if (issue_break)
    {
       set_instr(INSTR_CONTROL);
       ctrl_reg = ReadWriteData(PRACC | PROBEN | SETDEV | JTAGBRK );
       if (ReadWriteData(PRACC | PROBEN | SETDEV) & BRKST)  
          printf("<Processor Entered Debug Mode!> ... ");
       else  
          printf("<Processor did NOT enter Debug Mode!> ... ");
       printf("Done\n");
    } else printf("Skipped\n");


    // ----------------------------------
    // Clear Watchdog
    // ----------------------------------
    printf("Clearing Watchdog ... ");        
    if (issue_watchdog)
    {
       ejtag_write(0xb8000080,0);
       printf("Done\n");
    } else printf("Skipped\n");


    // ----------------------------------
    // Flash Chip Detection
    // ----------------------------------
    if (selected_fc != 0)
       sflash_config();
    else
       sflash_probe();


    // ----------------------------------
    // Execute Requested Operation
    // ----------------------------------
    if ((flash_size > 0) && (AREA_LENGTH > 0))
    {
       if (run_option == 1 )  run_backup(AREA_NAME, AREA_START, AREA_LENGTH);
       if (run_option == 2 )  run_erase(AREA_NAME, AREA_START, AREA_LENGTH);
       if (run_option == 3 )  run_flash(AREA_NAME, AREA_START, AREA_LENGTH);
       if (run_option == 4 );  // Probe was already run so nothing else needed
   
    }

    printf("\n\n *** REQUESTED OPERATION IS COMPLETE ***\n\n");

    chip_shutdown();

    return 0;
}


// **************************************************************************
// End of File
// **************************************************************************
