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

the interesting thing is the word at offset 0x1c, ``0x0c2664`` that is equal to the file
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

As is possible to see, some piece are only there to be used as constant pool, you can use radare
in visual mode and press ``dw`` to set them to data as 32bit value.

Right now I'm pretty sure that this firmware it loaded at ``0x40000000`` so let's tell radare2 this
in the project file with the line ``o "Pico SP-H03/tccboot42.bin" 0x40000000 r-x`` (this is a little
tricky since using the standard command from the radare2 shell and saving doesn't work, so I did change
manually the ``rc`` file to fix the issue).

Diving into the code it's possible to see some address that are strings, so we can move to the address ``0x40091de8``
and from the cursor mode press "ds" and define a string.

## Interrupts

At ``0x4002e2fc`` exists a function to disable IRQ/FIQ (see pg100 ARM11 specification), and at ``0x4002e2ec`` the opposite.

## UART

The most interesting part I want to find is the UART, from the specification I see that it is at address
``0xf0532000`` depending from the channel

```
/x 002.53f0
Searching 4 bytes in [0x40000000-0x400c25f4]
hits: 3
0x4002c440 hit1_0 002053f0
0x40030bc0 hit1_1 002053f0
0x40030cac hit1_2 002053f0
:> pd 10 @@hit*
            ;-- hit1_0:
            0x4002c440   *  002053f0       invalid
/ (fcn) interrupt_controller_something 188
|   interrupt_controller_something ();
|           ; var int local_ch @ fp-0xc
|           ; var int local_8h @ fp-0x8
|           0x4002c444      00482de9       push {fp, lr}
|           0x4002c448      04b08de2       add fp, sp, 4               ; add two values
|           0x4002c44c      08d04de2       sub sp, sp, 8               ; substract two values
|           0x4002c450      a8309fe5       ldr r3, [0x4002c500]        ; [0x4002c500:4]=0xf0401000 ; load from memory to register
|           0x4002c454      08300be5       str r3, [local_8h]          ; 8 ; store register into memory
|           0x4002c458      08301be5       ldr r3, [local_8h]          ; 8 ; load from memory to register
|           0x4002c45c      102093e5       ldr r2, [r3, 0x10]          ; load from memory to register
|           0x4002c460      08301be5       ldr r3, [local_8h]          ; 8 ; load from memory to register
|           0x4002c464      283093e5       ldr r3, [r3, 0x28]          ; load from memory to register
            ;-- hit1_1:
            0x40030bc0   *  002053f0       invalid
            0x40030bc4      002010f0       invalid
            0x40030bc8      04b02de5       str fp, [sp, -4]!           ; store register into memory
            0x40030bcc      00b08de2       add fp, sp, 0               ; add two values
            0x40030bd0      10309fe5       ldr r3, [0x40030be8]        ; [0x40030be8:4]=0x47e0ded8 ; load from memory to register
            0x40030bd4      003093e5       ldr r3, [r3]                ; load from memory to register
            0x40030bd8      0300a0e1       mov r0, r3                  ; move value between registers
            0x40030bdc      00d08be2       add sp, fp, 0               ; add two values
            0x40030be0      0008bde8       ldm sp!, {fp}
            0x40030be4      1eff2fe1       bx lr                       ; branches and exchanges cpu mode to 16 bits (thumb mode)
            ;-- hit1_2:
            0x40030cac   *  002053f0       invalid
            0x40030cb0      00482de9       push {fp, lr}
            0x40030cb4      04b08de2       add fp, sp, 4               ; add two values
            0x40030cb8      08d04de2       sub sp, sp, 8               ; substract two values
            0x40030cbc      08000be5       str r0, [fp, -8]            ; 8 ; store register into memory
        ,=< 0x40030cc0      060000ea       b 0x40030ce0                ; branches the program counter to dst (pc aka r15)
        |   0x40030cc4      08301be5       ldr r3, [fp, -8]            ; 8 ; load from memory to register
        |   0x40030cc8      0020d3e5       ldrb r2, [r3]               ; load byte from memory to register
        |   0x40030ccc      08301be5       ldr r3, [fp, -8]            ; 8 ; load from memory to register
        |   0x40030cd0      013083e2       add r3, r3, 1               ; add two values
```

and indeed the function just above have some operation with that register involved!

But now it's mandatory to add the correct memory mapping for the device: from the specification
we know that the memory regions are the following

| Address space | Device Name |
|---------------|-------------|
| ``0x00000000``-``0x00003fff`` | special region for instruction TCM |
| ``0x10000000``-``0x10003fff`` | on-chip 16kB memory |
| ``0x40000000``-``0x4fffffff`` | off-chip SDRAM chip |
| ``0xe0000000``-``0xe0003fff`` | internal boot ROM |
| ``0xf0000000``-``0xffffffff`` | on-chip peripherals |

so in radare2 we indicate this with the commands

```
[0x400328d4]> on malloc://0x4000 0x10000000 rw-
4
[0x400328d4]> omn 0x10000000 ram
[0x400328d4]> on malloc://0x0fffffff 0xf0000000 rw-
5
[0x400328d4]> omn 0xf0000000 peripherals
[0x400328d4]> om
 3 fd: 5 +0x00000000 0xf0000000 - 0xfffffffe rw- peripherals
 2 fd: 4 +0x00000000 0x10000000 - 0x10003fff rw- ram
 1 fd: 3 +0x00000000 0x40000000 - 0x400c25f3 r-x
```

To start we want to create a new flagspace for peripherals named ``per``
so to have directly name to refer

```
:> fs per
:> f per.uart_ch0 @ 0xf0532000
```

At ``0x40030fd8`` there are some comparisons with values that seem single byte char,
to make easier to read directly is possible to **hint** radare2 that we want to see
directly the char in the disassembly in visual mode with the sequence "dis"

## Wrong path

Looking at the code of the bootloader, particularly at [cr0.S](https://github.com/JeffreyLau/JJWD-K8_icsCream/blob/master/bootable/bootloader/lk/arch/arm/crt0.S)
is clear that all the binary file is intended to be loaded in memory; try yourself compiling the code

```
$ bootable/bootloader/lk
$ make tcc8920_evm_emmc EMMC_BOOT=1 TOOLCHAIN_PREFIX=/usr/bin/arm-linux-gnueabi-
```

The problem now is: how translate all the flag we set previously with the wrong addresses?
well, radare2 got us covered

```
f?
 ...
| fb [addr] [flag*]        move flags matching 'flag' to relative addr
 ...
```

## Setup

This is the start of the execution, setup the stack and stuffs...

## Main

Probably is at ``0x4002c68c`` where a series of init functions is called.
