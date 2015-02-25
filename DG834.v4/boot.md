The layout of the ``CFE`` in this router is described [here](http://wiki.openwrt.org/doc/techref/bootloader/cfe#bcm63xx_cfe).

[Seems](http://wiki.openwrt.org/doc/hardware/soc/soc.broadcom.bcm63xx#bootloader) that most of the others use CFE with a built-in LZMA decompressor. CFE is not using standard LZMA compression arguments, and most noticeably, changes the dictionary size, so beware.

Below the boot message as shown from the serial port

```
CFE version 1.0.37-6.8 for BCM96348 (32bit,SP,BE)
Build Date: Thu Jun  7 19:39:03 CST 2007 (root@Build_Server)
Copyright (C) 2000-2005 Broadcom Corporation.

Boot Address 0xbfc00000

Initializing Arena.
Initializing Devices.
Parallel flash device: name AM29LV320MB, id 0x2200, size 4096KB
CPU type 0x29107: 240MHz, Bus: 133MHz, Ref: 26MHz
Total memory: 16777216 bytes (16MB)

Total memory used by CFE:  0x80401000 - 0x80527BE0 (1207264)
Initialized Data:          0x8041F420 - 0x80420810 (5104)
BSS Area:                  0x80420810 - 0x80425BE0 (21456)
Local Heap:                0x80425BE0 - 0x80525BE0 (1048576)
Stack Area:                0x80525BE0 - 0x80527BE0 (8192)
Text (code) segment:       0x80401000 - 0x8041F414 (123924)
Boot area (physical):      0x00528000 - 0x00568000
Relocation Factor:         I:00000000 - D:00000000

Board IP address                  : 192.168.1.1:ffffff00  
Host IP address                   : 192.168.1.100  
Gateway IP address                :   
Run from flash/host (f/h)         : f  
Default host run file name        : vmlinux  
Default host flash file name      : bcm963xx_fs_kernel  
Boot delay (0-9 seconds)          : 1  
Board Id Name                     : 96348W3  
Psi size in KB                    : 24
Number of MAC Addresses (1-32)    : 2  
Base MAC Address                  : 12:34:56:ab:cd:ef  
Ethernet PHY Type                 : Internal
Memory size in MB                 : 16
CMT Thread Number                 : 0

*** Press any key to stop auto run (1 seconds) ***
Auto run second count down: 1
CFE> 
```

In order to reverse the bootloader we can use ``radare2`` setting the correct architecture and endianess

    $ radare2 -a mips -e cfg.bigendian=true

Incredibly the first 4 bytes of the flash are a jump instruction to the entry point:

```
[0x000009e8]> pd 20 @0x00 
            0x00000000    10000279     b 0x000009e8
```

after that we have a recurrent pattern

```
[0x000009e8]>pd 
            0x000009e8    0000e021     move gp, zero
        ,=< 0x000009ec    04110001     bal 0x000009f4
        |   0x000009f0    00000000     nop
        `-> 0x000009f4    00000000     nop
            0x000009f8    3c161fff     lui s6, 0x1fff
            0x000009fc    03f6f824     and ra, ra, s6
            0x00000a00    3c011fc0     lui at, 0x1fc0
            0x00000a04    03e1b022     sub s6, ra, at
            0x00000a08    3c041fc0     lui a0, 0x1fc0
            0x00000a0c    00962021     addu a0, a0, s6
       ,==< 0x00000a10    04110001     bal 0x00000a18
       |    0x00000a14    00000000     nop
       `--> 0x00000a18    00000000     nop
            0x00000a1c    3c1b0000     lui k1, 0x0
            0x00000a20    277bff94     addiu k1, k1, -108
            0x00000a24    037fd821     addu k1, k1, ra
            0x00000a28    3c01a000     lui at, 0xa000
            0x00000a2c    0361d825     or k1, k1, at
            0x00000a30    8f7b0000     lw k1, 0(k1)
            0x00000a34    0376d821     addu k1, k1, s6
            0x00000a38    3c01a000     lui at, 0xa000
            0x00000a3c    0361d825     or k1, k1, at
            0x00000a40    0360f809     jalr k1
```

in ``s6`` is saved the ``BOOT_OFFSET`` and then in ``k1`` will be placed the address of a jump table: in ``0xa10`` there is
a ``bl`` instruction  that save in ``ra`` the address +8 (i.e. in this case ``0xa18``); after that is subtracted ``-108`` to that
that corresponds to ``0x9ac``.

```
[0x000009e8]> px @0x9ac
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0x000009ac  9fc0 1230 9fc0 1504 9fc0 1450 9fc0 d27c  ...0.......P...|
0x000009bc  9fc0 d398 9fc0 d398 9fc0 d398 9fc0 d3a0  ................
0x000009cc  9fc0 1458 9fc0 d3a8 9fc0 d498 9fc0 152c  ...X...........,
0x000009dc  9fc0 1514 9fc0 151c 9fc0 d398 0000 e021  ...............!
0x000009ec  0411 0001 0000 0000 0000 0000 3c16 1fff  ............<...
0x000009fc  03f6 f824 3c01 1fc0 03e1 b022 3c04 1fc0  ...$<......"<...
0x00000a0c  0096 2021 0411 0001 0000 0000 0000 0000  .. !............
0x00000a1c  3c1b 0000 277b ff94 037f d821 3c01 a000  <...'{.....!<...
0x00000a2c  0361 d825 8f7b 0000 0376 d821 3c01 a000  .a.%.{...v.!<...
0x00000a3c  0361 d825 0360 f809 0000 0000 3c04 4845  .a.%.`......<.HE
0x00000a4c  3484 4c4f 0411 0001 0000 0000 0000 0000  4.LO............
0x00000a5c  3c1b 0000 277b ff54 037f d821 3c01 a000  <...'{.T...!<...
0x00000a6c  0361 d825 8f7b 0004 0376 d821 3c01 a000  .a.%.{...v.!<...
0x00000a7c  0361 d825 0360 f809 0000 0000 0411 0001  .a.%.`..........
0x00000a8c  0000 0000 0000 0000 3c1b 0000 277b ff1c  ........<...'{..
0x00000a9c  037f d821 3c01 a000 0361 d825 8f7b 000c  ...!<....a.%.{..
```
At the address we found ``0x9fc01230`` but we already now that we have to relocate, so the address in the firmware is ``0x1230``

```
[0x000009e8]> pd 20 @0x1230
            0x00001230    40806000     mtc0 zero, $12
            0x00001234    3c020040     lui v0, 0x40
            0x00001238    40826000     mtc0 v0, $12
            0x0000123c    40806800     mtc0 zero, $13
            0x00001240    00000000     nop
```

At ``0x15dc`` there is a compressed part the the bootloader should decompress and should be in ``LZMA`` format
but not in a standard format that ``lzma(1)`` understands (see also [this](http://w00tsec.blogspot.it/2015/02/firmware-forensics-diffs-timelines-elfs.html)). 

The first 24 bytes are constants between versions I analized

```
000015dc  5d 00 00 40 00 00 08 00  2f 3e 61 3f fb 7d ac 43  |]..@..../>a?.}.C|
000015ec  a5 74 50 73 17 c9 55 1f  ea 89 79 f1 13 91 64 f1  |.tPs..U...y...d.|
```

``0x5D`` is not a magic number, it's simply the [standard value](http://svn.python.org/projects/external/xz-5.0.3/doc/lzma-file-format.txt) (i.e. 3/0/2) for the ``properties`` field of a standard ``LZMA`` compressed stream.

looking in the interwebz I [found](http://sourceforge.net/p/sevenzip/discussion/45797/thread/e62d3075) a random guy with the same quest. With another search i finally found a ``CFE`` source code that seems to match the *fingerprint* individuated above, the project is here and happy to see that compiles out of the box

    https://github.com/guillaumelecerf/cfe_bcm63xx

The build process occurs in two steps: first the *ram* part is build with the real ``CFE`` (I think is like the second stage of the bootloader) and using the ``cmplzma`` tool is compressed and dumped as assembly into ``$(ROM_BUILD)/flashimg.S``; this file is
then added as binary array (delimited by symbols ``_binArrayStart`` and ``_binArrayEnd``) and used as dependency for the *rom* part (or first stage) of the bootloader.

Remember that the 32 bit before (big endian) the ``0x5D`` byte there is saved the address at which is supposed to be run the second stage (in the cases disassembled by me is ``0x80401000``), so keep in mind that in order to understand the real offsets.