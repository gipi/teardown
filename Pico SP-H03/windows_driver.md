# Windows Driver

```
$ file Pico\ SP-H03/vtcdrv/x64/VTC\ Driver\ Installer\ v5.0.0.3\ for\ x64.EXE
Pico SP-H03/vtcdrv/x64/VTC Driver Installer v5.0.0.3 for x64.EXE: PE32 executable (GUI) Intel 80386, for MS Windows, MS CAB-Installer self-extracting archive
$ binwalk Pico\ SP-H03/vtcdrv/x64/VTC\ Driver\ Installer\ v5.0.0.3\ for\ x64.EXE 

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             Microsoft executable, portable (PE)
73228         0x11E0C         PNG image, 256 x 256, 8-bit/color RGBA, non-interlaced
73269         0x11E35         Zlib compressed data, default compression
151900        0x2515C         Microsoft Cabinet archive data, 397310 bytes, 6 files
551644        0x86ADC         XML document, version: "1.0"
551706        0x86B1A         Copyright string: "Copyright (c) Microsoft Corporation -->"
551755        0x86B4B         Copyright string: "Copyright (c) Microsoft Corporation.  All rights reserved."
$ peres -a Pico\ SP-H03/vtcdrv/x64/VTC\ Driver\ Installer\ v5.0.0.3\ for\ x64.EXE
$ find resources/ -type f -print0 | xargs --null file
resources/icons/7.ico:         data
resources/icons/1.ico:         data
resources/icons/12.ico:        data
resources/icons/6.ico:         dBase IV DBT of @.DBF, block length 1024, next free block index 40, next free block 15066613, next used block 15000828
resources/icons/11.ico:        data
resources/icons/5.ico:         data
resources/icons/9.ico:         PNG image data, 256 x 256, 8-bit/color RGBA, non-interlaced
resources/icons/8.ico:         GLS_BINARY_LSB_FIRST
resources/icons/3.ico:         data
resources/icons/10.ico:        data
resources/icons/13.ico:        GLS_BINARY_LSB_FIRST
resources/icons/4.ico:         GLS_BINARY_LSB_FIRST
resources/icons/2.ico:         dBase IV DBT of @.DBF, block length 512, next free block index 40, next free block 2291109880, next used block 28872
resources/manifests/1.xml:     XML 1.0 document, ASCII text, with CRLF line terminators
resources/strings/83.rc:       data
resources/strings/76.rc:       data
resources/strings/63.rc:       data
resources/strings/85.rc:       data
resources/strings/77.rc:       data
resources/strings/80.rc:       data
resources/versions/1.rc:       data
resources/groupicons/3000.ico: data
resources/rcdatas/2486.rc:     ASCII text, with no line terminators
resources/rcdatas/2336.rc:     data
resources/rcdatas/2442.rc:     ASCII text, with no line terminators
resources/rcdatas/2414.rc:     data
resources/rcdatas/2398.rc:     ASCII text, with no line terminators
resources/rcdatas/2530.rc:     ASCII text, with no line terminators
resources/rcdatas/2320.rc:     Microsoft Cabinet archive data, many, 397310 bytes, 6 files, at 0x2c +A "dpinst.exe" +A "dpinst.xml", ID 6404, number 1, 39 datablocks, 0x1503 compression
resources/rcdatas/2542.rc:     ASCII text, with no line terminators
resources/rcdatas/2508.rc:     data
resources/rcdatas/2558.rc:     ASCII text, with no line terminators
resources/rcdatas/2358.rc:     data
resources/rcdatas/2378.rc:     ASCII text, with no line terminators
resources/rcdatas/2304.rc:     ASCII text, with no line terminators
resources/rcdatas/2472.rc:     data
resources/dialogs/2002.dlg:    data
resources/dialogs/2005.dlg:    data
resources/dialogs/2006.dlg:    data
resources/dialogs/2003.dlg:    data
resources/dialogs/2001.dlg:    data
resources/dialogs/2004.dlg:    data
resources/3001.bin:            RIFF (little-endian) data, AVI, 272 x 60, 10.00 fps, video: RLE 8bpp
```


