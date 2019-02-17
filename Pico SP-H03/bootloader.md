# TCC8900 bootloader

Looking at the start of the file ``TCCBoot4.2.rom`` is possible to see like 0x70 bytes of header

```
[0x00000000 [Xadvc] 0% 2856 Pico SP-H03/P-OBRNPWWC-1008.1/TCCBoot4.2.rom]> xc
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF  comment
0x00000000  1c00 00ea 0601 ffff 5665 723a 4c42 5f5f  ........Ver:LB__
0x00000010  91ae 839a 0000 0000 cd90 b83b 6426 0c00  ...........;d&..
0x00000020  0000 0000 0000 0000 0000 0000 0000 0000  ................
0x00000030  0000 0000 0000 0000 0000 0000 0000 0000  ................
0x00000040  0000 0000 0000 0000 0000 0000 0000 0000  ................
0x00000050  0000 0000 0000 0000 0000 0000 0000 0000  ................
0x00000060  0000 0040 0000 0000 5637 2020 3839 5858  ...@....V7  89XX
0x00000070  e0a0 0740 0ca7 0740 0f00 00ea 14f0 9fe5  ...@...@........
0x00000080  14f0 9fe5 14f0 9fe5 14f0 9fe5 14f0 9fe5  ................
0x00000090  14f0 9fe5 14f0 9fe5 e401 0040 e401 0040  ...........@...@
0x000000a0  e401 0040 e401 0040 e401 0040 e801 0040  ...@...@...@...@
0x000000b0  f801 0040 0000 e047 c8e6 ee47 08d2 9fe5  ...@...G...G....
```

the interesting thing is the word at offset 0x0c, ``0x0c2664`` that is equal to the file
size; remove the header and try to look at that

```
$ dd if=Pico\ SP-H03/P-OBRNPWWC-1008.1/TCCBoot4.2.rom of=Pico\ SP-H03/tccboot42.bin bs=1c skip=112
$ r2 -n -zz -a arm -b 32 Pico\ SP-H03/tccboot42.bin
 <press "v" and then "p">
[0x00000000 [xAdvc] 0% 255 Pico SP-H03/tccboot42.bin]> pd $r
            0x00000000      e0a00740       andmi sl, r7, r0, ror 1
            0x00000004      0ca70740       andmi sl, r7, ip, lsl 14
        ,=< 0x00000008      0f0000ea       b 0x4c                      ;[1]
        |   0x0000000c      14f09fe5       ldr pc, [0x00000028]        ; [0x28:4]=0x400001e4 ; '('
        |   0x00000010      14f09fe5       ldr pc, [0x0000002c]        ; [0x2c:4]=0x400001e4 ; ','
        |   0x00000014      14f09fe5       ldr pc, [0x00000030]        ; [0x30:4]=0x400001e4 ; '0'
        |   0x00000018      14f09fe5       ldr pc, [0x00000034]        ; [0x34:4]=0x400001e4 ; '4'
        |   0x0000001c      14f09fe5       ldr pc, [0x00000038]        ; [0x38:4]=0x400001e4 ; '8'
        |   0x00000020      14f09fe5       ldr pc, [0x0000003c]        ; [0x3c:4]=0x400001e8 ; '<'
        |   0x00000024      14f09fe5       ldr pc, [0x00000040]        ; [0x40:4]=0x400001f8 ; '@'
        |   0x00000028      e4010040       andmi r0, r0, r4, ror 3
        |   0x0000002c      e4010040       andmi r0, r0, r4, ror 3
        |   0x00000030      e4010040       andmi r0, r0, r4, ror 3
        |   0x00000034      e4010040       andmi r0, r0, r4, ror 3
        |   0x00000038      e4010040       andmi r0, r0, r4, ror 3
        |   0x0000003c      e8010040       andmi r0, r0, r8, ror 3
        |   0x00000040      f8010040       strdmi r0, r1, [r0], -r8
        |   0x00000044      0000e047       strbmi r0, [r0, r0]!
        |   0x00000048      c8e6ee47       strbmi lr, [lr, r8, asr 13]!
        `-> 0x0000004c      08d29fe5       ldr sp, [0x0000025c]        ; [0x25c:4]=0x10004000
            0x00000050      08029fe5       ldr r0, [0x00000260]        ; [0x260:4]=0x50078
            0x00000054      100f01ee       mcr p15, 0, r0, c1, c0, 0
            0x00000058      0000a0e3       mov r0, 0
            0x0000005c      150f07ee       mcr p15, 0, r0, c7, c5, 0
            0x00000060      160f07ee       mcr p15, 0, r0, c7, c6, 0
            0x00000064      170f08ee       mcr p15, 0, r0, c8, c7, 0
            0x00000068      00e08fe2       add lr, pc, 0
            0x0000006c      86b800eb       bl 0x2e28c                  ;[2]
            0x00000070      ec319fe5       ldr r3, [0x00000264]        ; [0x264:4]=0xf0101000
```

all seems to make sense: in particular the last instruction reported above seems to load
into ``r3`` the value ``0xf0101000`` that from the TCC8900 specification indicates as the
**Core Bus Configuration Register** (``CORECFG``).

Indeed seems that here is setting ``CORECFG`` and in particular the ``REMAP`` field,
composed of two bits, to ``01b``, i.e. remap ``0x00000000 – 0x0FFFFFFF`` to on-chip 16kB memory Region

```
            0x00000070      ec319fe5       ldr r3, [0x00000264]        ; [0x264:4]=0xf0101000 ; load from memory to register
            0x00000074      0140a0e3       mov r4, 1                   ; move value between registers
            0x00000078      0350e0e3       mvn r5, 3
            0x0000007c      006093e5       ldr r6, [r3]                ; load from memory to register
            0x00000080      057006e0       and r7, r6, r5              ; logical AND
            0x00000084      048087e1       orr r8, r7, r4              ; logical inclusive OR
            0x00000088      008083e5       str r8, [r3]                ; store register into memory
```

Furthermore, if look at offset ``0x0000004c`` you see

```
08d29fe5       ldr sp, [0x0000025c]        ; [0x25c:4]=0x10004000
```

but ``0x10004000`` is the upper limit of the on-chip 16kB memory region: it makes sense, the stack grows downwards!

