# BROM

It's possible to extract the boot rom (after 0x8000 bytes it repeats itself)

```
$ Q700/q700.py --adec Q700/adec09_1.bin dump 0xbfc00000 0x8000 > brom.bin
```

The first 32 bytes are the following

```
00000000: 1800 0010 0000 0000 4300 0000 2507 1120  ........C...%.. 
00000010: d610 0950 0000 0000 0000 0000 0000 0000  ...P............
00000020: 0000 0000 4143 746f 7340 4163 7469 6f6e  ....ACtos@Action
00000030: 7300 0000 0000 0000 0000 0000 0000 0000  s...............
00000040: 4657 2044 6570 742e 0000 0000 0000 0000  FW Dept.........
```

and it seems that it named ``ACtos``.

There are two main "stages" at different places that are loaded into
internal(?) RAM and executed, in order

| Starting address | Size | Relocation address | Description |
|------------------|------|--------------------|-------------|
| ``0xbfc00800``   | ``0x1000`` | ``0xb4062c00`` | probably looks for the ``mbrec`` and execute what is described [here](mbrec.md) |
| ``0xbfc05400``   | ``0x1df8`` | ``0xb4066000`` | builtin ADFU mode |

## Boot from storage

The code starting from ``0xb4062c00`` copy another ``0x800`` bytes from
``0xbfc073fc`` to address ``0xb4060400``: this is some routines used to interact
with the flash

## Builting ADFU

This enables the hardware interrupt 4 and connects it with the ``USB`` subsystem; set the exception
vectors base address to ``0xb4066000`` with spacing of ``0x200`` between
entries. This means that the address ``0xb4066800`` is the ``ISR`` for this
service.

It creates a device with vendor and products ids ``10d6:10d6``, create an
endpoint that accepts three type of ``CBW`` packets: **upload**, **execute** and
**info**; the general structure of this packet is the following

| Offset |  | Name |
|--------|--|-----|
| 0  | ``55 53 42 43`` | signature |
| 4  | ``xx xx xx xx`` | tag |
| 8  | ``yy yy yy yy`` | transferLength |
| c  | ``ww`` | flags |
| d  | ``zz`` | LUN |
| e  | ``1f`` | cmdLength |
| f  | ``<cmd>`` | cmd |
| 10 | ``AA AA AA AA`` | arg0 |
| 14 | ``BB BB BB BB`` | arg1 |
| 18 | ``CC CC CC CC`` | arg2 |
| 1c | ``kk kk`` | padding |

The service only cares about the ``signature``, the ``cmd`` and the necessary
fields, based on the ``cmd`` indicated.

| ``cmd`` | Description | ``arg0`` | ``arg1`` | ``arg2`` |
|---------|-------------|----------|----------|----------|
| ``0x5`` | upload encrypted code | destination address | size | must not be ``0x80`` |
| ``0x10`` | executed | entry point address | ignored | ignored |
| ``0xcc`` | obtain info | ignored | ignored | ignored |
