# Vodafone Gateway

## Serial

The pinout is

 - TX
 - VCC
 - GND
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

The pinout is the following

```
 10 GND      9  TDI
 8  nTRST??  7  (??)
 6  nSRTT??  5  TMS
 4  VCC      3  TDO
 2  GND      1  TCK
```

The orientation of the pinout is found checkig the row with the
two ``GND`` for contuinity.

It's possible to single stepping the CPU

```
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
