# Vodafone Gateway

## Serial

The pinout is

 - TX
 - GND
 - VCC
 - 
 - RX (near the capacitor)

Connecting to the serial gives nothing, only some random
messages

```
*** gStartRxDesc[0] = 0xA02D3000 
*** gBufferSizeBytes = 1280 
*** gStartTxDesc[0] = 0xA02D4000 
hal6358PcmInit 260 nextTxDesc = 0xA02D4000 
hal6358PcmInit 260 nextTxDesc = 0xA02D4008 
hal6358PcmInit 264 Ownership for TX desc not set. Use this buffer. 
PERF->IrqMask   = 0xA00A2CA4 
PERF->IrqMask1  = 0x00800000
```

## JTAG

The pinout (taken from the OpenWRT related page) is the following

```
        GND  10  9  TDI (orange)
   nTSRT(?)   8  7  N/C
   nSRST(?)   6  5  TMS (red)
        VCC   4  3  TDO (brown)
(black) GND   2  1  TCK (yellow)
```

The orientation of the pinout is found checkig the row with the
two ``GND`` for contuinity.

It doesn't work pain-free: using the ``bus pirate`` you have
to connect after the board is booted (at least one led is on)
otherwise does not halt correctly. Surfing the interwebz for
informations, some broadcom chip has for example a pin to enable
``JTAG`` (see [this](http://www.cypress.com/file/298756/download) at page 7
or search for the ``JTAG_SEL`` pin).

It's possible to single stepping the CPU

```
> halt
target state: halted
target halted in MIPS32 mode due to debug-request, pc: 0x00000000
> targets
    TargetName         Type       Endian TapName            State       
--  ------------------ ---------- ------ ------------------ ------------
 0* bcm6358.cpu        mips_m4k   big    bcm6358.cpu        halted
> reg pc
pc (/32): 0x80013640
> mdw 0x80013640 0x20
0x80013640: 8e020080 10400003 00000000 0040f809 00000000 27820008 8c420000 30420008 
0x80013660: 1040fff7 00000000 0c085de0 00000000 08004d96 27820008 8c830098 3c02cfff 
0x80013680: 3442ffe7 00621824 34630010 ac830098 00803821 8f840000 2403dfff 8c82000c 
0x800136a0: 00431024 ac82000c c3820008 3c01fffe 3421ffff 00411024 e3820008 10400026
```

and indeed these are ``MIPS`` opcodes

```
$ r2 -a mips -m 0x80013640  -e cfg.bigendian=true -c 'wx 8e020080 10400003 00000000 0040f809 00000000 27820008 8c420000 30420008 1040fff7' malloc://4096
[0x80013640]> pd 10
        .-> 0x80013640      8e020080       lw v0, 0x80(s0)
       ,==< 0x80013644      10400003       beqz v0, 0x80013654
       |!   0x80013648      00000000       nop
       |!   0x8001364c      0040f809       jalr v0
       |!   0x80013650      00000000       nop
       `--> 0x80013654      27820008       addiu v0, gp, 8
        !   0x80013658      8c420000       lw v0, (v0)
        !   0x8001365c      30420008       andi v0, v0, 8
        `=< 0x80013660      1040fff7       beqz v0, 0x80013640
            0x80013664      00000000       nop
```

It's possible to dump the ``CFE`` directly from the memory
from address ``0x80410000``

```
> dump_image HG553/cfe.raw 0x80401000 0x20000
dumped 131072 bytes in 1161.769775s (0.110 KiB/s)
```

Also the bootloader

```
> dump_image HG553/bootloader.raw 0xbe000000 0x10000
dumped 65536 bytes in 579.898376s (0.110 KiB/s)
$ binwalk HG553/bootloader.raw 

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
1288          0x508           CFE boot loader

```

```
> flash write_image erase HG553/cfe_headerless/cfe.bin 0xbe000000 bin
auto erase enabled
No working memory available. Specify -work-area-phys to target.
not enough working area available(requested 140)
Programming at 0xbe000000, count 0x00040000 bytes remaining
Programming at 0xbe000100, count 0x0003ff00 bytes remaining
 ...
Programming at 0xbe03ff00, count 0x00000100 bytes remaining
wrote 262144 bytes from file HG553/cfe.bin in 34213.093750s (0.007 KiB/s)
```

## Links

 - [OpenWRT related page](https://wiki.openwrt.org/toh/huawei/hg553)
 - [Debrick HG553](https://onetransistor.blogspot.it/2016/02/debrick-huawei-hg553-brcm6358-cfe.html)
 - [Alternate Register Names in MIPS](https://www.cs.umd.edu/class/sum2003/cmsc311/Notes/Mips/altReg.html)
