# TL-WR841N

## Firmware

```
$ binwalk TL-WR841N/wr841nv10_wr841ndv10_en_3_16_9_up_boot\(150310\).bin

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             TP-Link firmware header, firmware version: 0.-15473.3, image version: "", product ID: 0x0, product version: 138477584, kernel load address: 0x0, kernel entry point: 0x80002000, kernel offset: 4063744, kernel length: 512, rootfs offset: 761358, rootfs length: 1048576, bootloader offset: 2883584, bootloader length: 0
13440         0x3480          U-Boot version string, "U-Boot 1.1.4 (Mar 10 2015 - 15:00:39)"
13488         0x34B0          CRC32 polynomial table, big endian
14800         0x39D0          uImage header, header size: 64 bytes, header CRC: 0x8E2B46CA, created: 2015-03-10 07:00:39, image size: 35711 bytes, Data Address: 0x80010000, Entry Point: 0x80010000, data CRC: 0x72C78246, OS: Linux, CPU: MIPS, image type: Firmware Image, compression type: lzma, image name: "u-boot image"
14864         0x3A10          LZMA compressed data, properties: 0x5D, dictionary size: 33554432 bytes, uncompressed size: 93256 bytes
131584        0x20200         TP-Link firmware header, firmware version: 0.0.3, image version: "", product ID: 0x0, product version: 138477584, kernel load address: 0x0, kernel entry point: 0x80002000, kernel offset: 3932160, kernel length: 512, rootfs offset: 761358, rootfs length: 1048576, bootloader offset: 2883584, bootloader length: 0
132096        0x20400         LZMA compressed data, properties: 0x5D, dictionary size: 33554432 bytes, uncompressed size: 2219160 bytes
1180160       0x120200        Squashfs filesystem, little endian, version 4.0, compression:lzma, size: 2477651 bytes, 560 inodes, blocksize: 131072 bytes, created: 2015-03-10 07:25:11
```

```
$ ./mktplinkfw -i wr841nv10_wr841ndv10_en_3_16_9_up_boot\(150310\).bin 
File name              : wr841nv10_wr841ndv10_en_3_16_9_up_boot(150310).bin
File size              : 0x003e0200 /  4063744 bytes
Version 1 Header size  : 0x00000200 /      512 bytes
Header MD5Sum1         : 2e 10 c6 17 ef 43 31 2b 71 e0 03 33 95 27 b3 e4 (ok)
Header MD5Sum2         : f6 6b 51 40 2f 86 8c 7a 61 cd ab fb 83 2e 91 3d (purpose yet unknown, unchecked here)

Vendor name            : TP-LINK Technologies
Firmware version       : ver. 1.0
Hardware ID            : 0x08410010 (unknown)
Hardware Revision      : 0x00000001

Kernel data offset     : 0x00000200 /      512 bytes
Kernel data length     : 0x000b9e0e /   761358 bytes
Kernel load address    : 0x80002000
Kernel entry point     : 0x801943b0
Rootfs data offset     : 0x00100000 /  1048576 bytes
Rootfs data length     : 0x002c0000 /  2883584 bytes
Boot loader data offset: 0x00000000 /        0 bytes
Boot loader data length: 0x0000c38f /    50063 bytes
Total firmware length  : 0x003e0200 /  4063744 bytes
```

## Links

 - [Product page](https://www.tp-link.com/it/support/download/tl-wr841n/v10/)
 - [OpenWRT page](https://openwrt.org/toh/tp-link/tl-wr841nd)
 - [TP Link header](https://oldwiki.archive.openwrt.org/doc/techref/header)
 - [CVEs](https://www.cvedetails.com/vulnerability-list.php?vendor_id=11936&product_id=23482&version_id=0&page=1&hasexp=0&opdos=0&opec=0&opov=0&opcsrf=0&opgpriv=0&opsqli=0&opxss=0&opdirt=0&opmemc=0&ophttprs=0&opbyp=0&opfileinc=0&opginf=0&cvssscoremin=1&cvssscoremax=0&year=0&month=0&cweid=0&order=1&trc=3&sha=3bd1591ed25ebc92cae58c9f45417e81f43a35dd)
