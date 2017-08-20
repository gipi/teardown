# Firmware

Analysing the update firmware with ``radare2`` we can see that the file header
has a certain structure

```
[0x00000000 0% 5712 MiraScreen/mirawire_8252n_8M-16285000.gz]> xc
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF0123456789ABCDEF  comment
0x00000000  4163 7469 6f6e 7346 6972 6d77 6172 6500 5570 6461 7465 5665 723a 7631 2e33 3100  ActionsFirmware.UpdateVer:v1.31.
0x00000020  4348 4543 4b53 554d 0000 0000 0000 0000 4000 0000 c0ed 6600 0000 0000 5afa ad12  CHECKSUM........@.....f.....Z...
0x00000040  434f 4d50 5245 5353 0000 0000 0000 0000 0000 0000 0000 0000 00c8 1600 00ce 1600  COMPRESS........................
0x00000060  4144 4543 6164 6675 7300 0000 0000 0000 0004 0000 0020 0000 0000 04b4 0000 0000  ADECadfus............ ..........
0x00000080  4144 4655 6164 6675 7300 0000 0000 0000 0024 0000 1826 0000 0000 00a0 0000 0000  ADFUadfus........$...&..........
0x000000a0  4857 5343 6877 7363 0000 0000 0000 0000 004c 0000 2034 0000 0000 01a0 0000 0000  HWSChwsc.........L.. 4..........
0x000000c0  4e30 3830 6677 7363 0000 0000 0000 0000 0082 0000 2059 0000 0080 01a0 0000 0000  N080fwsc............ Y..........
0x000000e0  4e30 3830 6d62 7265 6300 0000 0000 0000 00dc 0000 0012 0000 0000 01a0 0040 04b4  N080mbrec....................@..
0x00000100  4e30 3830 6272 6563 0000 0000 0000 0000 00ee 0000 00f0 0000 0000 0000 1000 0000  N080brec........................
0x00000120  4649 524d 0000 0000 9a0b f0dc a174 bfc5 00de 0100 0000 1700 00ce 1600 dfc6 1600  FIRM.........t..................
0x00000140  4c49 4e55 5800 0000 0000 0000 0000 0000 0100 0000 0000 2000 0200 0000 0000 0000  LINUX................. .........
0x00000160  726f 6f74 6673 0000 0000 0000 0000 0000 00a6 1800 0050 4e00 0050 4e00 8300 0100  rootfs...............PN..PN.....
0x00000180  7672 616d 0000 0000 0000 0000 0000 0000 00a0 6600 004e 0000 0000 0800 0b00 0000  vram..............f..N..........
```

the first 16 bytes contains the **magic sequence** ``ActionsFirmare\0`` followed by another 16
bytes with the version of the firmware.

After that we can assume that are indicated the different section of the file; probably
can be described with the following struct

```
struct af {
    char name[16];
    uint32_t offset;
    uint32_t size;
    uint32_t unknown0;
    uint32_t unknown1;
};
```

this works up to ``FIRM`` section included (there are some discrepancies)
