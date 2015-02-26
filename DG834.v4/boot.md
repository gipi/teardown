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

In my case it was ``0x80401000`` so after decompress the second stage we can open it with radare2 with the following

```
o ./cfe_delzma.raw 0x80401000

```

The function of interest is the one at ``0x80407ec4``, I called it ``_download_callback`` that is execute when the download mode is
activated

```
[0x80401000]> pdf @ _download_callback 
/ (fcn) _download_callback 1608
| |||||||   0x80407ec4    27bdf9c0     addiu sp, sp, -0x640
| |||||||   0x80407ec8    24040001     addiu a0, zero, 1
| |||||||   0x80407ecc    24050001     addiu a1, zero, 1
| |||||||   0x80407ed0    afbf063c     sw ra, 0x63c(sp)
| |||||||   0x80407ed4    afbe0638     sw fp, 0x638(sp)
| |||||||   0x80407ed8    afb70634     sw s7, 0x634(sp)
| |||||||   0x80407edc    afb5062c     sw s5, 0x62c(sp)
| |||||||   0x80407ee0    afb1061c     sw s1, 0x61c(sp)
| |||||||   0x80407ee4    afb60630     sw s6, 0x630(sp)
| |||||||   0x80407ee8    afb40628     sw s4, 0x628(sp)
| |||||||   0x80407eec    afb30624     sw s3, 0x624(sp)
| |||||||   0x80407ef0    afb20620     sw s2, 0x620(sp)
| |||||||   0x80407ef4    0c101ea0     jal 0x407a80
| |||||||      0x00407a80() ; fcn.000003e8+4224664
| |||||||   0x80407ef8    afb00618     sw s0, 0x618(sp)
| |||||||   0x80407efc    00002821     move a1, zero
| |||||||   0x80407f00    0c101ea0     jal 0x407a80
| |||||||      0x00407a80() ; fcn.000003e8+4224664
| |||||||   0x80407f04    24040002     addiu a0, zero, 2
| |||||||   0x80407f08    3c048042     lui a0, 0x8042
| |||||||   0x80407f0c    0c101ebf     jal 0x407afc
| |||||||      0x00407afc() ; fcn.000003e8+4224788
| |||||||   0x80407f10    2484f534     addiu a0, a0, -0xacc
| |||||||   0x80407f14    27a405f0     addiu a0, sp, 0x5f0
| |||||||   0x80407f18    00002821     move a1, zero
| |||||||   0x80407f1c    0c104358     jal 0x410d60
| |||||||      0x00410d60() ; fcn.000003e8+4262264
| |||||||   0x80407f20    24060014     addiu a2, zero, 0x14
| |||||||   0x80407f24    27a40010     addiu a0, sp, 0x10
| |||||||   0x80407f28    00002821     move a1, zero
| |||||||   0x80407f2c    0c104358     jal 0x410d60
| |||||||      0x00410d60() ; fcn.000003e8+4262264
| |||||||   0x80407f30    240605dc     addiu a2, zero, 0x5dc
| |||||||   0x80407f34    27a40608     addiu a0, sp, 0x608
| |||||||   0x80407f38    00002821     move a1, zero
| |||||||   0x80407f3c    0c104358     jal 0x410d60
| |||||||      0x00410d60() ; fcn.000003e8+4262264
| |||||||   0x80407f40    24060006     addiu a2, zero, 6
| |||||||   0x80407f44    27a40610     addiu a0, sp, 0x610
| |||||||   0x80407f48    00002821     move a1, zero
| |||||||   0x80407f4c    0c104358     jal 0x410d60
| |||||||      0x00410d60() ; fcn.000003e8+4262264
| |||||||   0x80407f50    24060006     addiu a2, zero, 6
| |||||||   0x80407f54    3c048042     lui a0, 0x8042
| |||||||   0x80407f58    0c1047bf     jal 0x411efc
| |||||||      0x00411efc() ; fcn.000003e8+4266772
| |||||||   0x80407f5c    2484d564     addiu a0, a0, -0x2a9c     ; eth0
| |||||||   0x80407f60    0040a821     move s5, v0               ; s5 probably contains the file descriptor from which will read
| |||||||   0x80407f64    241e0001     addiu fp, zero, 1
| |||||||   0x80407f68    0000b821     move s7, zero
| ||||||`=< 0x80407f6c    04410003     bgez v0, 0x80407f7c
| ||||||    0x80407f70    00008821     move s1, zero
| ||||||    0x80407f74    24150001     addiu s5, zero, 1
| ||||||    0x80407f78    0000f021     move fp, zero
| ||||||    0x80407f7c    3c048042     lui a0, 0x8042
| ||||||    0x80407f80    0c10420a     jal 0x410828
| ||||||       0x00410828() ; fcn.000003e8+4260928
| ||||||    0x80407f84    2484df1c     addiu a0, a0, -0x20e4     ; Download mode ... press enter to stop
| |||||`==< 0x80407f88    0810212e     j 0x4084b8
| |||||     0x80407f8c    00000000     nop
| |||||     0x80407f90    0057102b     sltu v0, v0, s7           ; jump from 0x804084c0
| ||||`===< 0x80407f94    10400007     beqz v0, 0x80407fb4
| ||||      0x80407f98    02a02021     move a0, s5
| ||||      0x80407f9c    0c101eb1     jal 0x407ac4
| ||||         0x00407ac4() ; fcn.000003e8+4224732
| ||||      0x80407fa0    24040001     addiu a0, zero, 1
| ||||      0x80407fa4    0c101eb1     jal 0x407ac4
| ||||         0x00407ac4() ; fcn.000003e8+4224732
| ||||      0x80407fa8    24040002     addiu a0, zero, 2
| ||||      0x80407fac    0000b821     move s7, zero
| ||||      0x80407fb0    02a02021     move a0, s5
| ||||      0x80407fb4    27a50010     addiu a1, sp, 0x10
| ||||      0x80407fb8    0c1047fe     jal 0x411ff8
| ||||         0x00411ff8() ; fcn.000003e8+4267024               ; fcn(s5, struct10, sizeof(struct10))
| ||||      0x80407fbc    240605dc     addiu a2, zero, 0x5dc
| ||||      0x80407fc0    2842001a     slti v0, v0, 0x1a
| |||`====< 0x80407fc4    1440013c     bnez v0, check_console_input_and_back
| |||       0x80407fc8    26f70001     addiu s7, s7, 1
| |||       0x80407fcc    97a3001c     lhu v1, 0x1c(sp)          ; v1 = (struct ethhdr).h_proto
| |||       0x80407fd0    34028888     ori v0, zero, 0x8888      ; ETH_P_NFTP
| ||`=====< 0x80407fd4    14620138     bne v1, v0, check_console_input_and_back  ; if (h_proto != ETH_P_NFTP)
| ||        0x80407fd8    27b30010     addiu s3, sp, 0x10
| ||        0x80407fdc    27b00016     addiu s0, sp, 0x16
| ||        0x80407fe0    02602021     move a0, s3
| ||        0x80407fe4    02002821     move a1, s0
| ||        0x80407fe8    24060006     addiu a2, zero, 6
| ||        0x80407fec    0c10434e     jal 0x410d38
| ||           0x00410d38() ; fcn.000003e8+4262224               ; swap MAC addresses
| ||        0x80407ff0    3c148042     lui s4, 0x8042
| ||        0x80407ff4    02002021     move a0, s0
| ||        0x80407ff8    26855a60     addiu a1, s4, 0x5a60
| ||        0x80407ffc    0c10434e     jal 0x410d38
| ||           0x00410d38() ; fcn.000003e8+4262224
| ||        0x80408000    24060006     addiu a2, zero, 6
| ||        0x80408004    97a3001e     lhu v1, 0x1e(sp)          ; seems nftp_type
| ||        0x80408008    00031200     sll v0, v1, 8
| ||        0x8040800c    3042ff00     andi v0, v0, 0xff00
| ||        0x80408010    00031a02     srl v1, v1, 8
| ||        0x80408014    00431825     or v1, v0, v1
| ||        0x80408018    2c62000a     sltiu v0, v1, 0xa
| |`======< 0x8040801c    10400111     beqz v0, fcn.80408464
| |         0x80408020    00031080     sll v0, v1, 2
| |         0x80408024    3c038042     lui v1, 0x8042
| |         0x80408028    2463bf2c     addiu v1, v1, -0x40d4
| |         0x8040802c    00431021     addu v0, v0, v1
| |         0x80408030    8c420000     lw v0, (v0)
| |         0x80408034    00400008     jr v0                     ; indirect jump to 0x8040803c
| |         0x80408038    00000000     nop
| |         0x8040803c    26700018     addiu s0, s3, 0x18
| |         0x80408040    02002021     move a0, s0
| |         0x80408044    00002821     move a1, zero
| |         0x80408048    24060258     addiu a2, zero, 0x258
| |         0x8040804c    24023800     addiu v0, zero, 0x3800
| |         0x80408050    0c104358     jal 0x410d60
| |            0x00410d60() ; fcn.000003e8+4262264
| |         0x80408054    a6620016     sh v0, 0x16(s3)
| |         0x80408058    3c058042     lui a1, 0x8042
| |         0x8040805c    02002021     move a0, s0
| |         0x80408060    24a5f53b     addiu a1, a1, -0xac5
| |         0x80408064    0c10434e     jal 0x410d38
| |            0x00410d38() ; fcn.000003e8+4262224
| |         0x80408068    24060046     addiu a2, zero, 0x46
| `=======< 0x8040806c    0810211a     j 0x408468
|           0x80408070    9662000e     lhu v0, 0xe(s3)
|           0x80408074    96620010     lhu v0, 0x10(s3)
|           0x80408078    9664000e     lhu a0, 0xe(s3)
|           0x8040807c    00021a02     srl v1, v0, 8
|           0x80408080    00021200     sll v0, v0, 8
|           0x80408084    00431025     or v0, v0, v1
|           0x80408088    24420001     addiu v0, v0, 1
|           0x8040808c    24030001     addiu v1, zero, 1
|           0x80408090    a7a205f0     sh v0, 0x5f0(sp)
|           0x80408094    24020100     addiu v0, zero, 0x100
| ========< 0x80408098    14820003     bne a0, v0, 0x804080a8
|           0x8040809c    a7a30602     sh v1, 0x602(sp)
| ========< 0x804080a0    0810202b     j 0x4080ac
|           0x804080a4    24020004     addiu v0, zero, 4
|           0x804080a8    24020003     addiu v0, zero, 3
|           0x804080ac    a7a205fc     sh v0, 0x5fc(sp)
|           0x804080b0    27a205f0     addiu v0, sp, 0x5f0
| ========< 0x804080b4    08102119     j 0x408464
|           0x804080b8    a440000e     sh zero, 0xe(v0)
|           0x804080bc    97a20602     lhu v0, 0x602(sp)
|           0x804080c0    27b005f0     addiu s0, sp, 0x5f0
| ========< 0x804080c4    10400076     beqz v0, 0x804082a0
|           0x804080c8    24040005     addiu a0, zero, 5
|           0x804080cc    96620010     lhu v0, 0x10(s3)
|           0x804080d0    97a505f0     lhu a1, 0x5f0(sp)
|           0x804080d4    00021a00     sll v1, v0, 8
|           0x804080d8    3063ff00     andi v1, v1, 0xff00
|           0x804080dc    00021202     srl v0, v0, 8
|           0x804080e0    00621825     or v1, v1, v0
|           0x804080e4    0065102a     slt v0, v1, a1
| ========< 0x804080e8    1440006d     bnez v0, 0x804082a0
|           0x804080ec    00002021     move a0, zero
|           0x804080f0    00a3102a     slt v0, a1, v1
| ========< 0x804080f4    1440006a     bnez v0, 0x804082a0
|           0x804080f8    24040006     addiu a0, zero, 6
|           0x804080fc    96640014     lhu a0, 0x14(s3)
|           0x80408100    96650012     lhu a1, 0x12(s3)
|           0x80408104    00041200     sll v0, a0, 8
|           0x80408108    00051a00     sll v1, a1, 8
|           0x8040810c    3042ff00     andi v0, v0, 0xff00
|           0x80408110    00042202     srl a0, a0, 8
|           0x80408114    00441025     or v0, v0, a0
|           0x80408118    3063ff00     andi v1, v1, 0xff00
|           0x8040811c    00052a02     srl a1, a1, 8
|           0x80408120    00651825     or v1, v1, a1
|           0x80408124    00021100     sll v0, v0, 4
|           0x80408128    00439021     addu s2, v0, v1
| ========< 0x8040812c    16400005     bnez s2, 0x80408144
|           0x80408130    97a305fe     lhu v1, 0x5fe(sp)
|           0x80408134    97a205fe     lhu v0, 0x5fe(sp)
|           0x80408138    24420001     addiu v0, v0, 1
|           0x8040813c    a7a205fe     sh v0, 0x5fe(sp)
|           0x80408140    97a305fe     lhu v1, 0x5fe(sp)
|           0x80408144    96650016     lhu a1, 0x16(s3)
|           0x80408148    97a605fc     lhu a2, 0x5fc(sp)
|           0x8040814c    00031d00     sll v1, v1, 0x14
|           0x80408150    00052200     sll a0, a1, 8
|           0x80408154    3c02fff0     lui v0, 0xfff0
|           0x80408158    02431821     addu v1, s2, v1
|           0x8040815c    3084ff00     andi a0, a0, 0xff00
|           0x80408160    00052a02     srl a1, a1, 8
|           0x80408164    00629021     addu s2, v1, v0
|           0x80408168    24020003     addiu v0, zero, 3
|           0x8040816c    10c20006     beq a2, v0, 0x80408188
|           0x80408170    0085b025     or s6, a0, a1
|           0x80408174    24020004     addiu v0, zero, 4
|           0x80408178    10c2000a     beq a2, v0, 0x804081a4
|           0x8040817c    00000000     nop
| ========< 0x80408180    08102079     j 0x4081e4
|           0x80408184    9603000c     lhu v1, 0xc(s0)
|           0x80408188    00002021     move a0, zero
|           0x8040818c    0c101f87     jal 0x407e1c
|              0x00407e1c() ; fcn.000003e8+4225588
|           0x80408190    3c050040     lui a1, 0x40
| ========< 0x80408194    14400009     bnez v0, 0x804081bc
|           0x80408198    24020001     addiu v0, zero, 1
| ========< 0x8040819c    08102074     j 0x4081d0
|           0x804081a0    24040001     addiu a0, zero, 1
|           0x804081a4    3c05003d     lui a1, 0x3d
|           0x804081a8    34a5ffff     ori a1, a1, 0xffff
|           0x804081ac    0c101f87     jal 0x407e1c
|              0x00407e1c() ; fcn.000003e8+4225588
|           0x804081b0    3c040001     lui a0, 1
| ========< 0x804081b4    10400005     beqz v0, 0x804081cc
|           0x804081b8    24020002     addiu v0, zero, 2
|           0x804081bc    24020200     addiu v0, zero, 0x200
|           0x804081c0    24040007     addiu a0, zero, 7
| ========< 0x804081c4    081020a8     j 0x4082a0
|           0x804081c8    a7a20602     sh v0, 0x602(sp)
|           0x804081cc    24040001     addiu a0, zero, 1
|           0x804081d0    0c101eb1     jal 0x407ac4
|              0x00407ac4() ; fcn.000003e8+4224732
|           0x804081d4    a7a205fc     sh v0, 0x5fc(sp)
|           0x804081d8    24024e20     addiu v0, zero, 0x4e20
|           0x804081dc    af828048     sw v0, -0x7fb8(gp)
|           0x804081e0    9603000c     lhu v1, 0xc(s0)
|           0x804081e4    24020001     addiu v0, zero, 1
|           0x804081e8    1062000b     beq v1, v0, 0x80408218
|           0x804081ec    24020002     addiu v0, zero, 2
| ========< 0x804081f0    1462002b     bne v1, v0, 0x804082a0
|           0x804081f4    00002021     move a0, zero
|           0x804081f8    3402ffff     ori v0, zero, 0xffff
|           0x804081fc    0052102b     sltu v0, v0, s2
| ========< 0x80408200    14400005     bnez v0, 0x80408218
|           0x80408204    00000000     nop
|           0x80408208    96020000     lhu v0, (s0)
|           0x8040820c    24420001     addiu v0, v0, 1
| ========< 0x80408210    081020a8     j 0x4082a0
|           0x80408214    a6020000     sh v0, (s0)
|           0x80408218    96020000     lhu v0, (s0)
|           0x8040821c    24420001     addiu v0, v0, 1
|           0x80408220    a6020000     sh v0, (s0)
|           0x80408224    3c020001     lui v0, 1
| ========< 0x80408228    16420004     bne s2, v0, 0x8040823c
|           0x8040822c    3c02003e     lui v0, 0x3e
|           0x80408230    0c101e7a     jal 0x4079e8
|              0x004079e8() ; fcn.000003e8+4224512
|           ;-- hit6_6:
|           0x80408234    26845a60     addiu a0, s4, 0x5a60
|           0x80408238    3c02003e     lui v0, 0x3e
|           0x8040823c    3442fffe     ori v0, v0, 0xfffe
|           0x80408240    0052102b     sltu v0, v0, s2
| ========< 0x80408244    14400015     bnez v0, 0x8040829c
|           0x80408248    26740018     addiu s4, s3, 0x18
|           0x8040824c    00008021     move s0, zero
|           0x80408250    00008821     move s1, zero
| ========< 0x80408254    12400008     beqz s2, 0x80408278
|           0x80408258    00002821     move a1, zero
|           0x8040825c    0c10100d     jal 0x404034
|              0x00404034() ; fcn.000003e8+4209740
|           0x80408260    3204ffff     andi a0, s0, 0xffff
|           0x80408264    02512823     subu a1, s2, s1
|           0x80408268    02228821     addu s1, s1, v0
|           0x8040826c    0232102b     sltu v0, s1, s2
| ========< 0x80408270    1440fffa     bnez v0, 0x8040825c
|           0x80408274    26100001     addiu s0, s0, 1
| ========< 0x80408278    16320003     bne s1, s2, 0x80408288
|           0x8040827c    00000000     nop
|           0x80408280    26100001     addiu s0, s0, 1
|           0x80408284    00002821     move a1, zero
|           0x80408288    2610ffff     addiu s0, s0, -1
|           0x8040828c    3204ffff     andi a0, s0, 0xffff
|           0x80408290    02803021     move a2, s4
|           0x80408294    0c100ffd     jal 0x403ff4
|              0x00403ff4() ; fcn.000003e8+4209676
|           0x80408298    02c03821     move a3, s6
|           0x8040829c    00002021     move a0, zero
| ========< 0x804082a0    08102119     j 0x408464
|           0x804082a4    3091ffff     andi s1, a0, 0xffff
|           0x804082a8    26640018     addiu a0, s3, 0x18
|           0x804082ac    00002821     move a1, zero
|           0x804082b0    0c104358     jal 0x410d60
|              0x00410d60() ; fcn.000003e8+4262264
|           0x804082b4    24060258     addiu a2, zero, 0x258
|           0x804082b8    24100002     addiu s0, zero, 2
|           0x804082bc    a6600016     sh zero, 0x16(s3)
|           0x804082c0    02a02021     move a0, s5
|           0x804082c4    02602821     move a1, s3
|           0x804082c8    0c104820     jal 0x412080
|              0x00412080() ; fcn.000003e8+4267160
|           0x804082cc    24060040     addiu a2, zero, 0x40      ; NFTP_PROBE_LEN 0x40
|           0x804082d0    2610ffff     addiu s0, s0, -1
|           0x804082d4    0c104ad1     jal 0x412b44
|              0x00412b44() ; fcn.000003e8+4269916
|           0x804082d8    24040001     addiu a0, zero, 1
| ========< 0x804082dc    0601fff9     bgez s0, 0x804082c4
|           0x804082e0    02a02021     move a0, s5
|           0x804082e4    0c101e7a     jal 0x4079e8
|              0x004079e8() ; fcn.000003e8+4224512
|           ;-- hit6_7:
|           0x804082e8    26845a60     addiu a0, s4, 0x5a60
|           0x804082ec    0c1011bd     jal 0x4046f4
|              0x004046f4() ; fcn.000003e8+4211468
|           0x804082f0    00000000     nop
| ========< 0x804082f4    0810211a     j 0x408468
|           0x804082f8    9662000e     lhu v0, 0xe(s3)
|           0x804082fc    97a20602     lhu v0, 0x602(sp)
|           0x80408300    27a805f0     addiu t0, sp, 0x5f0
| ========< 0x80408304    10400053     beqz v0, 0x80408454
|           0x80408308    00002821     move a1, zero
|           0x8040830c    96620010     lhu v0, 0x10(s3)
|           0x80408310    97a405f0     lhu a0, 0x5f0(sp)
|           0x80408314    00021a00     sll v1, v0, 8
|           0x80408318    3063ff00     andi v1, v1, 0xff00
|           0x8040831c    00021202     srl v0, v0, 8
|           0x80408320    00621825     or v1, v1, v0
|           0x80408324    2482ffff     addiu v0, a0, -1
|           0x80408328    1043004e     beq v0, v1, fcn.80408464
|           0x8040832c    30b1ffff     andi s1, a1, 0xffff
| ========< 0x80408330    14830048     bne a0, v1, 0x80408454
|           0x80408334    24050006     addiu a1, zero, 6
|           0x80408338    97a305fc     lhu v1, 0x5fc(sp)
|           0x8040833c    24020001     addiu v0, zero, 1         ; maybe check for value == 1?
| ========< 0x80408340    14620007     bne v1, v0, 0x80408360
|           0x80408344    24020005     addiu v0, zero, 5
| ========< 0x80408348    081020dc     j 0x408370
|           0x8040834c    a7a205fc     sh v0, 0x5fc(sp)
|           0x80408350    24020200     addiu v0, zero, 0x200
|           0x80408354    24050009     addiu a1, zero, 9
| ========< 0x80408358    08102115     j 0x408454
|           0x8040835c    a5020012     sh v0, 0x12(t0)
|           0x80408360    24020002     addiu v0, zero, 2         ; maybe check for value == 2?
| ========< 0x80408364    14620003     bne v1, v0, 0x80408374
|           0x80408368    24020006     addiu v0, zero, 6
|           0x8040836c    a7a205fc     sh v0, 0x5fc(sp)
|           0x80408370    a7a005fe     sh zero, 0x5fe(sp)
|           0x80408374    96640014     lhu a0, 0x14(s3)
|           0x80408378    96650012     lhu a1, 0x12(s3)
|           0x8040837c    00041200     sll v0, a0, 8
|           0x80408380    00051a00     sll v1, a1, 8
|           0x80408384    3042ff00     andi v0, v0, 0xff00
|           0x80408388    00042202     srl a0, a0, 8
|           0x8040838c    00441025     or v0, v0, a0
|           0x80408390    3063ff00     andi v1, v1, 0xff00
|           0x80408394    00052a02     srl a1, a1, 8
|           0x80408398    00651825     or v1, v1, a1
|           0x8040839c    00021100     sll v0, v0, 4
|           0x804083a0    00433021     addu a2, v0, v1
| ========< 0x804083a4    14c00004     bnez a2, 0x804083b8
|           0x804083a8    00000000     nop
|           0x804083ac    9502000e     lhu v0, 0xe(t0)
|           0x804083b0    24420001     addiu v0, v0, 1
|           0x804083b4    a502000e     sh v0, 0xe(t0)
|           0x804083b8    96640016     lhu a0, 0x16(s3)
|           0x804083bc    9503000e     lhu v1, 0xe(t0)
|           0x804083c0    00041200     sll v0, a0, 8
|           0x804083c4    3042ff00     andi v0, v0, 0xff00
|           0x804083c8    00031d00     sll v1, v1, 0x14
|           0x804083cc    00042202     srl a0, a0, 8
|           0x804083d0    00442825     or a1, v0, a0
|           0x804083d4    00c31821     addu v1, a2, v1
|           0x804083d8    3c02fff0     lui v0, 0xfff0
|           0x804083dc    00623021     addu a2, v1, v0
|           0x804083e0    3c020001     lui v0, 1
|           0x804083e4    24a30001     addiu v1, a1, 1
|           0x804083e8    0046102b     sltu v0, v0, a2
| ========< 0x804083ec    10400015     beqz v0, 0x80408444
|           0x804083f0    00032842     srl a1, v1, 1
| ========< 0x804083f4    10a00013     beqz a1, 0x80408444
|           0x804083f8    00003821     move a3, zero
|           0x804083fc    3c02fffe     lui v0, 0xfffe
|           0x80408400    344b2000     ori t3, v0, 0x2000
|           0x80408404    240aff00     addiu t2, zero, -0x100
|           0x80408408    3c09a000     lui t1, 0xa000
|           0x8040840c    8d620000     lw v0, (t3)
|           0x80408410    00071840     sll v1, a3, 1
|           0x80408414    00732021     addu a0, v1, s3
|           0x80408418    004a1024     and v0, v0, t2
|           0x8040841c    00491025     or v0, v0, t1
|           0x80408420    00c21021     addu v0, a2, v0
|           0x80408424    00431021     addu v0, v0, v1
|           0x80408428    94430000     lhu v1, (v0)
|           0x8040842c    94820018     lhu v0, 0x18(a0)
| ========< 0x80408430    1462ffc7     bne v1, v0, 0x80408350
|           0x80408434    24e70001     addiu a3, a3, 1
|           0x80408438    00e5102b     sltu v0, a3, a1
| ========< 0x8040843c    1440fff3     bnez v0, 0x8040840c
|           0x80408440    00000000     nop
|           0x80408444    95020000     lhu v0, (t0)
|           0x80408448    00002821     move a1, zero
|           0x8040844c    24420001     addiu v0, v0, 1
|           0x80408450    a5020000     sh v0, (t0)
| ========< 0x80408454    08102119     j 0x408464
|           0x80408458    30b1ffff     andi s1, a1, 0xffff
|           0x8040845c    0c101ef1     jal 0x407bc4
|              0x00407bc4() ; fcn.000003e8+4224988
|           0x80408460    00000000     nop
|           ;-- fcn.80408464:
|           0x80408464    9662000e     lhu v0, 0xe(s3)
| ========< 0x80408468    14400005     bnez v0, 0x80408480
|           0x8040846c    24020200     addiu v0, zero, 0x200
|           0x80408470    02a02021     move a0, s5
|           0x80408474    27a50010     addiu a1, sp, 0x10
| ========< 0x80408478    0810212c     j 0x4084b0
|           0x8040847c    24060050     addiu a2, zero, 0x50
|           0x80408480    26640018     addiu a0, s3, 0x18
|           0x80408484    00002821     move a1, zero
|           0x80408488    24060258     addiu a2, zero, 0x258
|           0x8040848c    0c104358     jal 0x410d60
|              0x00410d60() ; fcn.000003e8+4262264
|           0x80408490    a6620016     sh v0, 0x16(s3)
|           0x80408494    00111a02     srl v1, s1, 8
|           0x80408498    00111200     sll v0, s1, 8
|           0x8040849c    00431025     or v0, v0, v1
|           0x804084a0    02a02021     move a0, s5
|           0x804084a4    27a50010     addiu a1, sp, 0x10
|           0x804084a8    24060040     addiu a2, zero, 0x40      ; NFTP_PROBE_LEN = 0x40
|           0x804084ac    a6620018     sh v0, 0x18(s3)
|           0x804084b0    0c104820     jal 0x412080
|              0x00412080() ; fcn.000003e8+4267160
|           0x804084b4    00008821     move s1, zero
/ (fcn) check_console_input_and_back 8
|           ;-- fcn.804084b8:
|           0x804084b8    0c104920     jal 0x412480
|              0x00412480() ; fcn.000003e8+4268184
|           0x804084bc    00000000     nop
| ========< 0x804084c0    1040feb3     beqz v0, 0x80407f90       ; if a key is pressed then v0 = 1
|           0x804084c4    8f828048     lw v0, -0x7fb8(gp)
/ (fcn) fcn.804084c8 68
| ========< 0x804084c8    13c00004     beqz fp, 0x804084dc
|           0x804084cc    8fbf063c     lw ra, 0x63c(sp)
|           0x804084d0    0c1047d8     jal 0x411f60
|              0x00411f60() ; fcn.000003e8+4266872
|           0x804084d4    02a02021     move a0, s5
|           0x804084d8    8fbf063c     lw ra, 0x63c(sp)
|           0x804084dc    8fbe0638     lw fp, 0x638(sp)
|           0x804084e0    8fb70634     lw s7, 0x634(sp)
|           0x804084e4    8fb60630     lw s6, 0x630(sp)
|           0x804084e8    8fb5062c     lw s5, 0x62c(sp)
|           0x804084ec    8fb40628     lw s4, 0x628(sp)
|           0x804084f0    8fb30624     lw s3, 0x624(sp)
|           0x804084f4    8fb20620     lw s2, 0x620(sp)
|           0x804084f8    8fb1061c     lw s1, 0x61c(sp)
|           0x804084fc    8fb00618     lw s0, 0x618(sp)
|           0x80408500    00001021     move v0, zero
|           0x80408504    03e00008     jr ra
\           0x80408508    27bd0640     addiu sp, sp, 0x640
```
