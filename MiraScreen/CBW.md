# Update process CBW

This page contains the notes about the reversing of the protocol used by the Mirascreen
for its update process.

It's based on the **Bulk only transport** for the ``USB`` **Mass Storage class**, [here](https://www.usb.org/sites/default/files/usbmassbulk_10.pdf)
the specification. In particular what interest us is the **Command Block Wrapper** (``CBW``)
packet.

It's available a capture of the update communication in this [issue](https://github.com/c3c/miracast/issues/1)

Using wireshark is possible to filter the packets using a description like

```
usbms && scsi.spc.opcode == 0xb0 && usbms.dCBWTag == 0x77c05c94
```

The protocol seems an updated version of what is described [here](https://web.archive.org/web/20160418032428/http://wiki.s1mp3.org/USB_modes/) for the S1MP3.

## Dumped stream

```
host > device
0000   55 53 42 43 88 00 00 00 00 20 00 00 00 00 10 05
0010   00 00 04 b4 00 20 00 00 00 00 00 00 00 00 00

host < device
0000   55 53 42 53 00 00 00 00 00 00 00 00 00

>
0000   55 53 42 43 00 00 00 00 00 00 00 00 00 00 00 10
0010   00 00 04 b4 00 00 00 00 00 00 00 00 00 00 00

<
0000   55 53 42 53 00 00 00 00 00 00 00 00 00

>
0000   55 53 42 43 88 00 00 00 04 00 00 00 00 00 10 05
0010   f0 1f 04 b4 04 00 00 00 80 00 00 80 00 00 00

<
0000   55 53 42 53 88 00 00 00 00 00 00 00 00

>
0000   55 53 42 43 88 00 00 00 08 26 00 00 00 00 10 05
0010   00 00 00 a0 08 26 00 00 00 00 00 00 00 00 00

<
0000   55 53 42 53 88 00 00 00 00 00 00 00 00

>
0000   55 53 42 43 00 00 00 00 00 00 00 00 00 00 00 10
0010   00 00 00 a0 00 00 00 00 00 00 00 00 00 00 00

<
0000   55 53 42 53 00 00 00 00 00 00 00 00 00

>
0000   55 53 42 43 88 00 00 00 a0 27 00 00 00 00 10 05
0010   00 00 01 a0 a0 27 00 00 00 00 00 00 00 00 00

<
0000   55 53 42 53 88 00 00 00 00 00 00 00 00

>
0000   55 53 42 43 00 00 00 00 00 00 00 00 00 00 10 b0
0010   00 00 01 a0 00 00 00 00 58 46 00 00 00 00 00

<
0000   55 53 42 53 00 00 00 00 00 00 00 00 00

>
0000   55 53 42 43 10 92 a7 00 02 00 00 00 00 00 10 b0
0010   00 00 00 00 00 00 02 00 ae 6a 00 00 00 00 00

<
0000   50 00

< (packet n.97)
0000   55 53 42 53 10 92 a7 00 00 00 00 00 00
```

## Taxonomy

What follows is the commands via USB that the device in download mode
accepts.

It seems that when the CBW accepts a size in reality the two LSB bytes are
the size that we are going to write and the two MSB are the size we are
going to read.

The role of the **tag** and **subCmd** is not clear at all, they seem
magic values.

### Cmd 05 Tag88: upload

           0052f154 55 53 42 43     ddw       43425355h               signature
           0052f158 88 00 00 00     ddw       88h                     tag
           0052f15c 00 00 00 00     ddw       0h                      transferLength
           0052f160 00              db        0h                      flags
           0052f161 00              db        0h                      LUN
           0052f162 10              db        10h                     cmdLength
           0052f163 05 < dstAddr >  db[15]                            cmdBlock
                    <   size  > 00 
                    00 00 00 00 00

followed by a certain numbers of requests that will contain the payload

### Cmd b0 Tag 00: execute driver

           0052f194 55 53 42 43     ddw       43425355h               signature
           0052f198 00 00 00 00     ddw       0h                      tag
           0052f19c 00 00 00 00     ddw       0h                      transferLength
           0052f1a0 00              db        0h                      flags
           0052f1a1 00              db        0h                      LUN
           0052f1a2 10              db        10h                     cmdLength
           0052f1a3 b0 <exec addr>  db[15]                            cmdBlock
                    00 00 00 00 58 
                    46 00 00 00 00

you need to upload a driver in order to execute it.

### Cmd 10 Tag 00: execute {ADEC,ADFU}adfus

           0052f174 55 53 42 43     ddw       43425355h               signature
           0052f178 00 00 00 00     ddw       0h                      tag
           0052f17c 00 00 00 00     ddw       0h                      transferLength
           0052f180 00              db        0h                      flags
           0052f181 00              db        0h                      LUN
           0052f182 00              db        0h                      cmdLength
           0052f183 10 <exec addr>  db[15]                            cmdBlock
                    00 00 00 00 00 
                    00 00 00 00 00

## Cmd b0 Tag ffffffff: boot check process

           0052f2f4 55 53 42 43     ddw       43425355h               signature
           0052f2f8 ff ff ff ff     ddw       FFFFFFFFh               tag
           0052f2fc 00 02 00 00     ddw       200h                    transferLength
           0052f300 00              db        0h                      flags
           0052f301 00              db        0h                      LUN
           0052f302 10              db        10h                     cmdLength
           0052f303 b0 00 00 00 00  db[15]                            cmdBlock
                    00 00 00 00 36 
                    6f 00 00 00 00

it returns 0x200 bytes 

from a pcap we have

```
0040   6d 62 72 63 00 00 aa 55 66 4d 66 4d 00 00 00 00   mbrc..ªUfMfM....
0050   00 00 06 08 00 00 00 00 00 00 00 00 00 00 00 00   ................
0060   6d 62 72 63 01 00 aa 55 66 4d 66 4d 00 00 00 00   mbrc..ªUfMfM....
0070   00 00 06 08 00 00 00 00 00 00 00 00 00 00 00 00   ................
0080   6d 62 72 63 02 00 aa 55 66 4d 66 4d 00 00 00 00   mbrc..ªUfMfM....
0090   00 00 06 08 00 00 00 00 00 00 00 00 00 00 00 00   ................
00a0   6d 62 72 63 03 00 aa 55 66 4d 66 4d 00 00 00 00   mbrc..ªUfMfM....
00b0   00 00 06 08 00 00 00 00 00 00 00 00 00 00 00 00   ................
00c0   62 72 65 63 00 00 aa 55 35 3e 35 3e 00 00 00 00   brec..ªU5>5>....
00d0   00 00 06 08 00 00 00 00 00 00 00 00 00 00 00 00   ................
00e0   62 72 65 63 01 00 aa 55 35 3e 35 3e 00 00 00 00   brec..ªU5>5>....
00f0   00 00 06 08 00 00 00 00 00 00 00 00 00 00 00 00   ................
0100   62 72 65 63 02 00 aa 55 35 3e 35 3e 00 00 00 00   brec..ªU5>5>....
0110   00 00 06 08 00 00 00 00 00 00 00 00 00 00 00 00   ................
0120   62 72 65 63 03 00 aa 55 35 3e 35 3e 00 00 00 00   brec..ªU5>5>....
0130   00 00 06 08 00 00 00 00 00 00 00 00 00 00 00 00   ................
0140   d4 36 05 61 76 7d 49 79 c9 6b 0d 16 e5 36 a6 46   Ô6.av}IyÉk..å6¦F
0150   fa 0c e7 33 4e 01 bf 02 ec 70 95 00 00 6e bc 0f   ú.ç3N.¿.ìp...n¼.
0160   ff 6e 84 3c fe 27 a1 67 f2 67 05 19 a6 60 67 66   ÿn.<þ'¡gòg..¦`gf
0170   a9 37 01 73 90 6f 51 7a 2e 74 e7 25 b9 29 7d 2e   ©7.s.oQz.tç%¹)}.
0180   f7 1d ea 0a e1 33 ba 43 67 25 6a 78 31 54 99 4b   ÷.ê.á3ºCg%jx1T.K
0190   96 69 af 55 21 07 c7 79 55 14 dd 16 99 5b 99 1f   .i¯U!.ÇyU.Ý..[..
01a0   6d 77 24 35 56 5a e9 05 28 4f 38 77 90 7c 21 19   mw$5VZé.(O8w.|!.
01b0   13 0e 01 1e da 75 95 1b 01 16 93 49 49 55 16 52   ....Úu.....IIU.R
01c0   53 02 60 2a d8 41 c3 5e 77 67 ac 52 0b 56 23 3e   S.`*ØAÃ^wg¬R.V#>
01d0   af 2b 42 3c d3 5e 71 1a 14 5d 66 56 b2 01 37 69   ¯+B<Ó^q..]fV².7i
01e0   da 28 0a 30 1f 0e 1f 07 d5 57 45 12 2c 7e 05 46   Ú(.0....ÕWE.,~.F
01f0   3c 74 00 1f 66 6a 50 21 ae 7c f3 47 fd 74 83 0c   <t..fjP!®|óGýt..
0200   72 48 d0 31 25 70 0f 10 03 01 bd 57 bb 43 6c 28   rHÐ1%p....½W»Cl(
0210   cc 54 08 03 2c 56 67 1b 30 47 16 6b 93 7c be 37   ÌT..,Vg.0G.k.|¾7
0220   cd 41 9d 11 21 36 ea 32 02 4b 13 2f c2 36 3c 19   ÍA..!6ê2.K./Â6<.
0230   ae 05 67 43 fd 04 2b 04 3b 5e ee 1e aa 55 35 3e   ®.gCý.+.;^î.ªU5>
```

that the application prints with the following format string

```
"name    chksum   chksum  MaxBit TtalBit BCHType Reclaim  \r\n"
```

then 4 times

```
"%s%d   0x%-4x   0x%-4x "
" %2d     %3d     %3d     %4d  \r\n"
```

and another 4 times the same last two and at the end

```
"VaildMbrcNum:%d,VaildBrecNum:%d  \r\n"
```

### Cmd b0 Tag 77c05c94: get HwSC info

           0052f1d4 55 53 42 43     ddw       43425355h               signature
           0052f1d8 94 5c c0 77     ddw       77C05C94h               tag
           0052f1dc <   size  >     ddw       0h                      transferLength
           0052f1e0 00              db        0h                      flags
           0052f1e1 00              db        0h                      LUN
           0052f1e2 10              db        10h                     cmdLength
           0052f1e3 b0 00 00 00 00  db[15]                            cmdBlock
                    00 00 <size>80 
                    c4 00 00 00 00  <--- 0xc480 seems get info

where ``size`` is ``0x50``.

It's supposed to be called after uploaded and run the ``hwscHWSC`` driver; it returns
the following

```
0000   48 57 63 39 00 41 13 00 00 00 20 06 05 20 46 36   HWc9.A.... .. F6
0010   34 38 00 00 00 00 00 00 00 00 00 00 00 00 00 00   48..............
0020   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
0030   00 00 00 00 00 00 00 00 00 00 a5 a5 00 00 00 00   ..........¥¥....
0040   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
```

at offset ``0x0e`` there is the string describing the type of flash
and the dword starting at ``0x3a`` is probably the size (must be shifted left by 9)
of the flash.

### Cmd b0 Tag 77c05c94: get FWsc info

The ``size`` parameter is ``0x90``. The response from a wireshark capture
seems like this

```
0000   46 57 d6 10 63 ff 31 33 2e 30 31 2e 30 30 2e 30   FWÖ.cÿ13.01.00.0
0010   30 30 30 55 00 00 00 00 41 63 74 69 6f 6e 73 20   000U....Actions 
0020   53 65 6d 69 63 6f 6e 64 75 63 74 6f 72 00 00 00   Semiconductor...
0030   00 00 00 00 00 00 00 00 40 00 08 00 08 00 02 20   ........@...... 
0040   20 20 20 20 20 00 00 00 00 00 00 00 00 00 00 00        ...........
0050   00 00 00 00 00 00 00 00 0a 0b 06 14 aa 55 00 00   ............ªU..
0060   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
0070   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
0080   00 d8 03 00 00 00 00 00 00 00 00 00 00 00 00 00   .Ø..............
```

### Cmd b0 Tag A79210: get length

           0052f1b4 55 53 42 43     ddw       43425355h               signature
           0052f1b8 10 92 a7 00     ddw       A79210h                 tag
           0052f1bc 02 00 00 00     ddw       2h                      transferLength
           0052f1c0 00              db        0h                      flags
           0052f1c1 00              db        0h                      LUN
           0052f1c2 10              db        10h                     cmdLength
           0052f1c3 b0 00 00 00 00  db[15]                            cmdBlock
                    00 00 02 00 ae 
                    6a 00 00 00 00 <--- 0x6aae seems get length

the response has in its first two bytes the length of the info response.

### Cmd b0 tag 0D: upload flash

```
0000   55 53 42 43 0d 00 00 00 00 00 01 00 00 00 10 b0
0010   00 70 02 00 00 00 80 00 1c 6a 00 01 00 00 00

0000   55 53 42 43 0d 00 00 00 00 00 01 00 00 00 10 b0
0010   80 70 02 00 00 00 80 00 1c 6a 00 01 00 00 00

0000   55 53 42 43 0d 00 00 00 00 00 01 00 00 00 10 b0
0010   00 71 02 00 00 00 80 00 1c 6a 00 01 00 00 00
```

```
0000   55 53 42 43
       0d 00 00 00
       00 00 01 00
       00
       00
       10
       b0 00 70 02 00
       00 00 80 00 1c
       6a 00 01 00 00 00
```

### Cmd b0 tag ffffffff subCmd 6f37: download from flash

download 512 bytes from the flash(?)

```
0000   55 53 42 43 ff ff ff ff 00 02 00 00 00 00 10 b0
0010   04 00 00 00 1f 00 00 00 37 6f 00 00 00 00 00
       '-----------'--- these seems to indicate an offset
```

Starting from the couple 0 0 it seems to download a multisector boot
with 0xd +1 packets for 4 times

```
00000000: 4D 75 6C 74 69 53 65 63  74 6F 72 42 6F 6F 74 00  MultiSectorBoot.
00000010: 04 00 00 00 00 00 00 00  00 40 04 B4 00 00 00 00  .........@......
00000020: 00 00 00 00 01 00 00 00  0D 00 00 00 00 00 00 00  ................
00000030: 01 00 00 00 01 00 00 00  0D 00 00 00 00 00 00 00  ................
00000040: 02 00 00 00 01 00 00 00  0D 00 00 00 00 00 00 00  ................
00000050: 03 00 00 00 01 00 00 00  0D 00 00 00 00 00 00 00  ................
00000060: 40 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  @...............
00000070: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000080: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000090: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000000A0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000000B0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000000C0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000000D0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000000E0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000000F0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000100: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000110: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000120: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000130: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000140: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000150: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000160: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000170: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000180: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000190: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000001A0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000001B0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000001C0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000001D0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000001E0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000001F0: 00 00 00 00 41 63 74 42  72 6D AA 55 02 CB DB 66  ....ActBrm.U...f
```

### Cmd b0 tag fffff subcmd 6f36: reset mbrc

This packet returns the mbrc

```
0000   55 53 42 43 ff ff ff ff 00 02 00 00 00 00 10 b0
0010   00 00 00 00 00 00 00 00 36 6f 00 00 00 00 00
```

```
00000000: 6D 62 72 63 00 00 AA 55  D7 93 D7 93 00 00 00 00  mbrc...U........
00000010: 00 00 06 08 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000020: 6D 62 72 63 01 00 AA 55  D7 93 D7 93 00 00 00 00  mbrc...U........
00000030: 00 00 06 08 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000040: 6D 62 72 63 02 00 AA 55  D7 93 D7 93 00 00 00 00  mbrc...U........
00000050: 00 00 06 08 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000060: 6D 62 72 63 03 00 AA 55  D7 93 D7 93 00 00 00 00  mbrc...U........
00000070: 00 00 06 08 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000080: 62 72 65 63 00 00 AA 55  3F 7F 3F 7F 00 00 00 00  brec...U?.?.....
00000090: 00 00 06 08 00 00 00 00  00 00 00 00 00 00 00 00  ................
000000A0: 62 72 65 63 01 00 AA 55  3F 7F 3F 7F 00 00 00 00  brec...U?.?.....
000000B0: 00 00 06 08 00 00 00 00  00 00 00 00 00 00 00 00  ................
000000C0: 62 72 65 63 02 00 AA 55  3F 7F 3F 7F 00 00 00 00  brec...U?.?.....
000000D0: 00 00 06 08 00 00 00 00  00 00 00 00 00 00 00 00  ................
000000E0: 62 72 65 63 03 00 AA 55  3F 7F 3F 7F 00 00 00 00  brec...U?.?.....
000000F0: 00 00 06 08 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000100: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000110: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000120: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000130: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000140: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000150: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000160: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000170: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000180: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
00000190: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000001A0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000001B0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000001C0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000001D0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000001E0: 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  ................
000001F0: 00 00 00 00 41 63 74 42  72 6D AA 55 02 CB DB 66  ....ActBrm.U...f
```

but from the serial is possible to see

```
######Erase_Tool_init ,0 2010-03-08 #####

***AL1213 1101,fwsc.bin Ver Apr 25 2017 Tim:11:32:50,1*****

mbrc data blk:0,checksum:93d7 93d7   7kb ECC: 0, 0,Reclaim:6,8
mbrc data blk:1,checksum:93d7 93d7   7kb ECC: 0, 0,Reclaim:6,8
mbrc data blk:2,checksum:93d7 93d7   7kb ECC: 0, 0,Reclaim:6,8
mbrc data blk:3,checksum:93d7 93d7   7kb ECC: 0, 0,Reclaim:6,8
brec data blk:0,checksum:7f3f 7f3f 128kb ECC: 0, 0,Reclaim:6,8
brec data blk:1,checksum:7f3f 7f3f 128kb ECC: 0, 0,Reclaim:6,8
brec data blk:2,checksum:7f3f 7f3f 128kb ECC: 0, 0,Reclaim:6,8
brec data blk:3,checksum:7f3f 7f3f 128kb ECC: 0, 0,Reclaim:6,8
```

so I think is erasing the flash(?).

### Cmd b0 tag 38f688 subcmd b586: maybe close connection

```
        0052f214          55 53 42        CBW_t
                          43 88 f6 
                          38 00 00 
           0052f214 55 53 42 43     ddw       43425355h               signature                                             
           0052f218 88 f6 38 00     ddw       38F688h                 tag
           0052f21c 00 00 00 00     ddw       0h                      transferLength
           0052f220 00              db        0h                      flags
           0052f221 00              db        0h                      LUN
           0052f222 10              db        10h                     cmdLength
           0052f223 b0 00 00 00 00  db[15]                            cmdBlock
                    00 00 00 00 86 
                    b5 00 00 00 00
```

from the serial only this output

```
afcfl fwsc n!
```

and the device disappears.
