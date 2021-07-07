# Reversing the flash dump

The flash dump is analyzed has the following md5

```
716c4263fb7ca44cd3606ab1874c06eb
```

since it's in the ``SPI`` flash and the actual user data is in
the SD card it's assumed that the former remains untouched
and varies only in a new revision.

## booting process

the boot process: loading the first 512 bytes

```
00000000: 0900 0010 0000 0000 1000 0000 0100 0000  ................
00000010: 0100 0000 0301 29b0 0000 0000 0000 0000  ......).........
00000020: 002a 06b4 4cb5 0000 01b0 083c 1c80 0835  .*..L......<...5
00000030: 0100 0924 0000 09ad 06b4 1d3c 007f bd37  ...$.......<...7
00000040: 0002 0424 1000 0524 0228 8570 2328 a400  ...$...$.(.p#(..
00000050: 06b4 063c 002a c624 06b4 1a3c 0004 5a37  ...<.*.$...<..Z7
00000060: 09f8 4003 0200 0824 06b4 043c 002a 8424  ..@....$...<.*.$
00000070: 001e 0524 4a80 010d 0000 0000 0300 4010  ...$J.........@.
00000080: 0000 0000 0b00 0010 0000 0000 06b4 083c  ...............<
00000090: aa55 093c 5a5a 2935 0000 09ad ffff 0834  .U.<ZZ)5.......4
000000a0: 22b0 093c 0800 2935 0000 28ad 808a 010d  "..<..)5..(.....
000000b0: 0000 0000 06b4 1a3c 0004 5a37 09f8 4003  .......<..Z7..@.
000000c0: 0300 0824 06b4 1a3c 0004 5a37 09f8 4003  ...$...<..Z7..@.
000000d0: 0900 0824 c0bf 083c 0800 098d 0b63 0124  ...$...<.....c.$
000000e0: 0200 2111 0050 0a35 0054 0a35 fc1f 0824  ..!..P.5.T.5...$
000000f0: 06b4 0b3c 0060 6b35 2160 0a01 0020 093c  ...<.`k5!`... .<
00000100: 2548 2201 fc1f 69ad 0000 488d 0400 4a25  %H"...i...H...J%
00000110: 0000 68ad fcff 4c15 0400 6b25 04e0 6821  ..h...L...k%..h!
00000120: 0800 0001 0000 0000 2158 8500 feff 6b25  ........!X....k%
00000130: 0000 6d95 feff 6b25 0000 6895 aa55 0a24  ..m...k%..h..U.$
00000140: 0800 0a15 2160 0000 0000 6895 2160 8801  ....!`....h.!`..
00000150: fdff 6415 feff 6b25 ffff 8c31 0200 8d11  ..d...k%...1....
00000160: 0000 0224 0100 0224 0800 e003 0000 0000  ...$...$........
00000170: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000180: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000190: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000001a0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000001b0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000001c0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000001d0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000001e0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000001f0: 0000 0000 4163 7442 726d aa55 3bb3 86ce  ....ActBrm.U;...
```

(where you note the ``ActBrm`` signature at the end), we have at the start
a jump to offset ``0x28``

```
ram:00000000               09 00 00 10     b                  FUN_00000028
ram:00000004               00 00 00 00     _nop
```

where the real startup happens; ``ghidra`` tries its best to come up
with something useful (below you can see the result after some reworking)

```c
void FUN_00000028(void)

{
  int iVar1;
  undefined4 s;
  dword *start;
  undefined4 *_dst;
  undefined4 *end;
  undefined4 *dst;
  
  _DAT_b001801c = 1;
  (*(code *)0xb4060400)(0x200,0x1e00,0xb4062a00);
  iVar1 = func_0x04060128(0xb4062a00,0x1e00);
  if (iVar1 == 0) {
    uRamb4060000 = 0x55aa5a5a;
    _DAT_b0220008 = 0xffff;
    func_0x04062a00();
  }
  (*(code *)0xb4060400)();
  uRamb4067ffc = (*(code *)0xb4060400)();
  start = (undefined4 *)&DAT_bfc05000;
  if (_DAT_bfc00008 != 0x630b) {
    start = (undefined4 *)&DAT_bfc05400;
  }
  end = start + 0x7ff;
  uRamb4067ffc = uRamb4067ffc | 0x20000000;
  dst = (undefined4 *)0xb4066000;
  do {
    _dst = dst;
    s = *start;
    start = start + 1;
    *_dst = s;
    dst = _dst + 1;
  } while (start != end);
  /* WARNING: Could not recover jumptable at 0x00000120. Too many branches */
  /* WARNING: Treating indirect jump as call */
  (*(code *)(_dst + -0x7fe))();
  return;
}
```

Seeing also the remaining part of the code seems that this code
will be loaded around address ``0xb406xxxx`` but if you use the
``0000`` as the starting address you quickly realize that something
doesn't match up: in particular the function at ``0xb4060400`` that
is in the middle of a well defined function.

With some experimentation around offsets the most plausible address is
``0xb4062a00`` that is the argument passed at the "not-matching-function":
my educated guess is that the function is loading, starting from offset ``0x200``
``0x1e00`` bytes (up to the partitions) at address ``0xb4062a00``, check something and then jump
to that address.

Since at offset ``0x128`` there is a function that is recognizable as a form of ``crc``

```c

int crc(dword *addr,int offset)

{
  bool bVar1;
  dword *ptr;
  short c;
  
  ptr = (dword *)((int)addr + offset + -4);
  c = 0;
  if (*(short *)ptr == 0x55aa) {
    do {
      c = c + *(short *)ptr;
      bVar1 = ptr != addr;
      ptr = (dword *)((int)ptr + -2);
    } while (bVar1);
    if (c == *(short *)((int)addr + offset + -2)) {
      return 0;
    }
  }
  return 1;
}

```

I'm assuming that at very start of the bootup process, the first 512 bytes 
of the flash are loaded at address ``0xb4060000``
with some other routines at address ``0xb4060400`` and then happens what is described above.

**Note:** the stack pointer is set at address ``0xb4067f00``, obviously the decompiler
cannot show that in ``C``.

**TL;DR:** in ``ghidra`` load the flash dump at ``0xb4062800``, probably there exists a
routine that move the flash data at that address and then jump to execute the
function at ``0xb4062a00``.

It's interesting this note in the
[rockbox's wiki](https://www.rockbox.org/wiki/ATJ213xFirmware#Booting_sequence) about the boot sequence
of the ``ATJ213x``:

    On power on the CPU begin execution at VA=0xbfc00000 (PA=0x1fc00000) where BROM
    is mapped. BROM loads MBRCF*.BIN stored at the beginning of nand flash into iram
    (VA=0xb4040000, PA=0x14040000), check signature and checksum and passes control
    to the just loaded code. MBRCF* initializes caches, clocks, DRAM, loads BREC*
    loader to the DRAM (VA=0x80000000, PA=0x00000000) and passes control to the
    BREC. BREC checks logical structure of flash, loads and passes control to uCOS
    kernel. If at any stage of load chain en error is encountered 'Hard DFU' handler
    stored at 0xbfc05400 (0x1ffc bytes) in BROM is copied to 0xb4046800 and CPU
    jumps to this handler.

in particular this last part seems that code after the failing of the ``crc`` check.

### Loading partitions

Now starts the business: it's a little difficult to describe the code so what
follows is a description of some important functions

At offset ``0x1254`` there is a function that reuses the original function
at ``0xb4060400``

```c
void map_sectors(int offset,dword *address,int length)

{
  FUN_b4060400(offset << 9,length << 9,(dword)address);
  return;
}
```

and it seems to use a **sector** as a unit (here it's implied that 512 bytes
is the size of such unit).

This function is used to start mapping the partition descriptions found at the offset ``0x2000``.
Then the checksum is calculated and compared with the value at ``0x23f4``.

Another important function is

```c
int load_partition_type(int type,dword **callback)

{
  dword *address;
  uint checksum;
  int ret;
  uint index;
  uint next_index;
  
  index = 0;
  while( true ) {
    next_index = index + 1;
    if (*(char *)&PARTITION_ADDRESS[index].type == type) break;
    index = next_index;
    if (0x1e < next_index) {
      *callback = NULL;
      return -1;
    }
  }
  map_sectors(PARTITION_ADDRESS[index].offset,&MEM_TMP_START,1);
  address = (dword *)tlb_configure_for_type
                               (type,DWORD_b4064c08,
                                ((MEM_TMP_END - MEM_TMP_START) + 0x203 >> 9) +
                                (PARTITION_ADDRESS[index].size >> 9));
  *callback = address;
  ret = -3;
  if (address != NULL) {
    map_sectors(PARTITION_ADDRESS[index].offset,address,
                PARTITION_ADDRESS[index].size >> 9);
    checksum = checksum_plus((byte *)*callback,PARTITION_ADDRESS[index].size,4);
    if (PARTITION_ADDRESS[index].checksum != checksum) {
      *callback = NULL;
      return -2;
    }
    zero((byte *)MEM_TMP_START,MEM_TMP_END);
    ret = 0;
    *callback = (dword *)MEM_CALLBACK;
  }
  return ret;
}
```

(edited to remove useless parameters); in practice it takes a partition identified
by a specific **type**, it maps it and returns the entry point of the loaded module.

The ``tlb_configure_for_type()`` configures the ``MMU`` and for this level of analysis
it's not important.

At this point the code is loading the module "startup.bin" and after that the module
named "brecf03.bin" with the following array of arguments:

 - the index of the ``TLB``(?)
 - the address where the calling module has been loaded (our ``0xb4062a00``)
 - some hardcoded address, probably read only data
 - some unidentified value
 - the entry point of the "storage.bin" module (mapped just before this)
 - the ``tlb_configure_for_type()`` address

## Partitions

At offset ``0x2000`` we see a block that resembles a partition
following this organization in memory:

```c
struct partition_t {
	char label[12];
	uint32_t type;
	uint32_t _unknown;
	uint32_t block_offset; /* block size of 512 bytes */
	uint32_t size; /* in bytes */
	uint32_t checksum; /* to be confirmed */
};
```

From each section of the partition is possible to extract raw binaries (that we'll call "modules"
in the following) that have
a common "header" (the first four dwords) with information about their loading:

 - upper limit of the memory used
 - max extension of memory needed
 - base address
 - entry point

| Label | Description | load address | type |
|-------|-------------|--------------|------|
| ``brecf03.bin`` | boot record ??? | ``0xc3080000`` | 0 |
| ``welcome.bin`` | | | 1 |
| ``storage.bin`` | | ``0xc30a0000`` | 3 |
| ``upg.bin`` | upgrade code | ``0xc0400000`` | 5 |
| ``startup.bin`` | bootstrap ? | ``0xb4064e00`` | 9 |
| ``fb.bin`` | configure framebuffer ??? | ``0xc0200000`` | 0xa |
| ``shutoff.bin`` | shutoff :) | `` `` | 0xc |
| ``key.bin`` | | | 0xd |
| ``mfp.bin`` | configure gpios ??? | ``0xc0280000`` | 0x16 |

## ``startup.bin``

It's the first called from the bootloader, it seems to check some configuration
strings and when are found use this to setup GPIOs accordingly.

## ``brecf03.bin``

The most important is the ``brec`` one, it sets a jump table at address
``0xc3080e00`` that is called from the other binaries to perform common service;
take note that the tenth entry is the ``log`` function!

This extracted binaries are only a little portion of the total content of the
flash, at ``0xb40c0000`` (offset ``0x80000``, 512KB) starts something else.

![entropy](entropy.png)

## ``upg.bin``

It looks for ``/mnt/udisk/us227a.upg``, probably the code indicates the specific
version of ``SDK`` used to build the system.

 - https://github.com/Pachouli/2819p
 - 
