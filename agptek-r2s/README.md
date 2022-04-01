# Agptek-RS

[Product page](http://www.agptek.com/index.php/product/agptek-r2s-clip-mp3-player-mini-lossless-sports-music-player-with-fmrecording-no-internal-memory-support-up-to-64gb-blue/)

| Component | Description |
|-----------|-------------|
| core | ATJ2127 LQFP-64 (7x7mm) |
| Flash | GD25Q128C (16M) |
| FM Radio tuner | RDA5807M |

The 7th pin from the right, on the side of the battery, is the reset signal connected
to a button on the side of the device.

## USB

If you connect the device to a computer via USB you obtain the following log

```
usb 2-5: new high-speed USB device number 23 using xhci_hcd
usb 2-5: New USB device found, idVendor=10d6, idProduct=1101, bcdDevice= 1.00
usb 2-5: New USB device strings: Mfr=1, Product=2, SerialNumber=3
usb 2-5: Product: HS USB FlashDisk
usb 2-5: Manufacturer: ACTIONS
usb 2-5: SerialNumber: 4512482ADF0FEEEE
usb-storage 2-5:1.0: USB Mass Storage device detected
scsi host6: usb-storage 2-5:1.0
scsi 6:0:0:0: Direct-Access     ACTIONS  USB DISK FOB 2.0 0    PQ: 0 ANSI: 0 CCS
sd 6:0:0:0: Attached scsi generic sg3 type 0
sd 6:0:0:0: [sdd] Media removed, stopped polling
sd 6:0:0:0: [sdd] Attached SCSI removable disk
```

if instead you press ``PLAY`` during the plug you obtain

```
usb 2-5: USB disconnect, device number 23
usb 2-6: new high-speed USB device number 24 using xhci_hcd
usb 2-6: New USB device found, idVendor=10d6, idProduct=10d6, bcdDevice= 1.00
usb 2-6: New USB device strings: Mfr=0, Product=0, SerialNumber=0
```

that should be the ``ADFU`` mode.

## Firmware

The firmware starts with the following header

```
00000000: 1122 3344 5566 7788 99aa bbcc ddee ff75  ."3DUfw........u
00000010: 00c8 a200 0002 0000 7ee1 3fad f8b0 2eaf  ........~.?.....
00000020: 6749 b985 5f63 4e5e 8e2e 214a 099a 96c0  gI.._cN^..!J....
00000030: 034d b77c 212e 87ef 8a00 03b0 9530 a954  .M.|!........0.T
```

It seems that the following command generates an sqlite file

```
$ /opt/rockbox/utils/atj2137/atjboottool/atjboottool --fw R2S-AG-V1.012017.07.22.fw
Header
  Signature: 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 75 Ok
  FW size: 10668032  Ok
  Block size: 512 Ok
  Version: 7e (Ver. 3)
  Unknown: 0xe1 Ok
  Signature: 3f ad f8 b0 2e af 67 49 b9 85 5f 63 4e 5e 8e 2e Ok
Firmware does not seem to have a tail
Crypto
  Block A: 6
  Block B: 23
Block A
  Check: Pass
BlockA
  00 04 00 01 7f 3d de 86 ed 52 1d 4e 8b 02 e2 6b  |.....=...R.N...k|
  f6 9a 11 3a 74 c0 dd 4b ad 1b 6e 44 9d 1c 8d bf  |...:t..K..nD....|
  d4 08 25 30 9a 9b 96 05 48 02 34 4f ba 70 20 45  |..%0....H.4O.p E|
  39 02 da 98 c7 ed bc 9d 2f 18 25 41 be 08 46 65  |9......./.%A..Fe|
  8c b8 83 f9 e5 b4 41 52 3a 09 99 49 9a 17 74 b6  |......AR:..I..t.|
  75 50 ed ee cf 8e 0c 7f 9a d6 10 4f 87 63 80 3b  |uP.........O.c.;|
  2e 2a 8a b0 2f ec 88 70 87 aa 72 4b a4 17 6f be  |.*../..p..rK..o.|
  01 6c 02 9a c7 4c db 0c 5b a8 3e 42 90 2b 78 d0  |.l...L..[.>B.+x.|
  9e b7 db d7 16 01 ee de f1 9f 18 4d 97 0c 5e 5d  |...........M..^]|
  bc 45 d1 de a2 bd 32 36 e2 f0 ff 48 83 76 81 b9  |.E....26...H.v..|
  ec c2 b9 b5 1d 88 f1 b3 7a c3 e6 4f b0 8d 5f 72  |........z..O.._r|
  01 a1 c8 83 fe 13 b1 6e 4e 01 50 41 84 d9 dd 24  |.......nN.PA...$|
  8b 2e c5 a5 df 5c b7 75 73 ab 99 44 8a e4 9e 25  |.....\.us..D...%|
  58 88 eb fa cc 26 a6 44 42 d5 a1 46 ba 97 ad 1c  |X....&.DB..F....|
  37 78 34 90 95 2b 43 46 66 2a 99 49 bb 40 d2 73  |7x4..+CFf*.I.@.s|
  8c d1 2d b8 4b 35 b8 1f 91 c0 a8 40 ab d9 36 be  |..-.K5.....@..6.|
  5b e1 e8 82 62 81 2b 7f 96 d8 86 49 b6 91 29 71  |[...b.+....I..)q|
  f9 67 38 3c 02 00 05 00 00 00 05 05 00 00 01 00  |.g8<............|
  e1 07 07 0d 0c 2e 2e 00 28 41 63 74 69 6f 6e 73  |........(Actions|
  20 53 65 6d 69 63 6f 6e 64 75 63 74 6f 72 20 43  | Semiconductor C|
  6f 2e 2c 4c 74 64 2c 20 54 6f 64 64 2d 5a 68 61  |o.,Ltd, Todd-Zha|
  6f 0e 50 6f 72 74 61 62 6c 65 44 65 76 69 63 65  |o.PortableDevice|
  28 02 b8 13 9f a9 ce c2 cc 41 b4 62 b6 0c 28 42  |(........A.b..(B|
  75 7d 02 1e 25 5e f7 3d e5 4a 9d 8c 23 4a 8f aa  |u}..%^.=.J..#J..|
  85 53 85 d4 47 b9 59 b3 32 48 8d 97 e3 e5 9f 0f  |.S..G.Y.2H......|
  91 a7 f3 2b f4 d7 9a 2d 96 48 bf 98 bd 3b 14 e6  |...+...-.H...;..|
  16 3b de f2 2a 3d 8f 2b 4f 4c aa 49 f2 b4 8f b2  |.;..*=.+OL.I....|
  45 d0 c8 46 6a 82 5c 6a 01 43 84 75 ad 23 35 04  |E..Fj.\j.C.u.#5.|
  17 59 65 f3 b1 f3 8f b5 bc 4d a8 b4 91 32 6a 45  |.Ye......M...2jE|
  3a ed 49 71 46 6a 4b 58 48 40 80 cb 60 57 5b 38  |:.IqFjKXH@..`W[8|
  39 8f ce 79 b9 1d 5e e2 83 4f b8 18 5d da dc cb  |9..y..^..O..]...|
  2b 4a d6 45 1c 40 6a 34 f4 4f a5 b6 b8 29 c8 32  |+J.E.@j4.O...).2|
  a4 2c 26 7f f7 2b ae 38 d6 40 9a bf f2 99 0f 19  |.,&..+.8.@......|
  64 0a de 73 86 b1 29 ff 3e 4c a5 20 15 2e 73 e8  |d..s..).>L. ..s.|
  96 52 5c 52 67 04 f9 2f 3d 47 83 16 64 57 3d 78  |.R\Rg../=G..dW=x|
  29 c3 6e b7 c6 de 01 4f c9 45 8e ca 32 c4 83 51  |).n....O.E..2..Q|
  91 ec b6 68 06 49 fc 88 66 40 8b 83 c3 9a 3e 3c  |...h.I..f@....><|
  8e 47 7c 67 d6 81 61 4c 28 46 8d 90 3a 3a ac 22  |.G|g..aL(F..::."|
  9e 79 2b fc dd 0a 43 0a 09 48 a7 bd 15 82 48 cb  |.y+...C..H....H.|
  3d 09 ef 81 a1 1a 2e 18 85 4e a4 7f 7d b9 11 5f  |=........N..}.._|
  52 4d 7d 31 d6 cc 20 72 da 4b 8d 02 f9 1d bc fd  |RM}1.. r.K......|
  8f 01 10 e3 62 90 18 eb 29 4c b9 62 0c 3f 9b 44  |....b...)L.b.?.D|
  20 c9 61 45 16 03 52 4c b3 4c b2 2e eb 87 91 a7  | .aE..RL.L......|
  35 99 7e a7 7c 82 65 95 dd 4b 94 87 b6 b7 31 a3  |5.~.|.e..K....1.|
  3d 0f fd 58 85 41 ff b9 be 4c 80 27 7e 0e 2c 9f  |=..X.A...L.'~.,.|
  55 e9 5d 2f bc 2b d8 45 73 45 98 b7 0f 31 33 0d  |U.]/.+.EsE...13.|
  57 95 d0 e9 a5 d7 16 91 6d 4d 99 fe 82 33 ba 06  |W.......mM...3..|
  aa cd 74 e1 ad 02 70 68 5f 42 98 7b d6 4c 30 67  |..t...ph_B.{.L0g|
  01 1e c0 03 ed fb dd 68 67 4a b3 28 f4 b7 3e f7  |.......hgJ.(..>.|
  68 88 29 a3 71 3d 42 36 7d 4a 8e bc e2 cf 51 8f  |h.).q=B6}J....Q.|
  f5 66 1a c4 34 dc 15 4f 6e 48 84 b8 cf 6f 1c 63  |.f..4..OnH...o.c|
  d9 2d d1 6f e2 fa 63 34 83 4a 8c 11 ec 42 94 d8  |.-.o..c4.J...B..|
  56 25 1e 45 6e c4 5d d3 e5 4d ae f4 ba 8a c0 74  |V%.En.]..M.....t|
  02 a9 19 4e 3b c3 13 ce d1 45 ab 4e 0e cd a7 fa  |...N;....E.N....|
  85 a0 ce be 80 ad 63 43 58 45 b7 2c b9 00 d5 84  |......cCXE.,....|
  51 e5 ab 7f f6 67 8c f8 d9 4c 8e 94 40 b6 8d d1  |Q....g...L..@...|
  dc ed a2 b0 2d 33 4d c1 e7 43 08 ee 82 3e 04 40  |....-3M..C...>.@|
  9a dd f8 50 db 0c 95 cd 8e 1a b0 82 b6 b3 ed 4c  |...P...........L|
  69 4b 02 c8 18 25 87 6b 8a f8 93 ce 00 f7 bd 30  |iK...%.k.......0|
  f8 5e 97 7e 98 27 84 7d 55 03 dc 80 0f 05 21 eb  |.^.~.'.}U.....!.|
  01 c1 b7 e7 4c 1f 90 e7 96 d9 01 4c d2 c7 f3 3b  |....L......L...;|
  62 9e 2b a8 44 b4 46 ba 19 cd ac 99 ad c5 cc fe  |b.+.D.F.........|
  76 5f 42 be 99 2b c4 c5 51 b5 83 01 4f 21 c2 7a  |v_B..+..Q...O!.z|
  2b 93 6c 24 7a d6 3a 4c 6a 84 d9 0f 02 ee a0 18  |+.l$z.:Lj.......|
  Decode Info:
    Nr Bits: 233
    Nr Bytes: 30
    Nr Bytes 2: 30
    Nr Words: 60
    Nr Words 2: 60
    Nr DWords: 8
    Size: 32
  Info: Pass
  Word: 1 Ok
Block B
  Word: 1 Ok
  Check: Pass
Main
  Byte: 3 Ok
  Crypto bits copy: Pass
  Crypto 4: Pass
  Word: 251 Ok
  Byte: 2 Ok
  DWord: 2056 Ok
  DWord: 8 Ok
  Byte: 190 Ok
  Decode block: Pass
  Compare: Pass
ATJ2127:
  Rounds: 16
File does not looks like an ATJ2127 firmware
Descrambling to R2S-AG-V1.012017.07.22.afi... Ok
$ file R2S-AG-V1.012017.07.22.afi 
R2S-AG-V1.012017.07.22.afi: SQLite 3.x database, last written using SQLite version 0
```
