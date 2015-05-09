My intention wasn't to install ``openwrt`` but to debrick this router that died three years ago.

Connecting to serial an trying ``tftp`` didn't work so I connected the serial with a ``bus pirate``, I reached the ``CFE``
bootloader and put the system in ``download mode``

```
CFE> d
Download mode ... press enter to stop
```

From a computer connected to the first LAN port i launched the ``nftp`` command

```
# ./nftp -u eth0 DG834GV4_V5.01.16_src/openwrt-dg834gt.img 
Sending frame on eth0 (34:64:a9:75:c6:b6)
Found a router at 12:34:56:ab:cd:ef
Router is a YFM720
Current version is 00
Max upgrade size is 65572 kb
Could not find "magic hardware header" in this image.
Uploading this is not recommended but press Enter if you wish to continue.

Erasing flash, this will take around ten seconds...
Upgrading:    2957312/2957312 bytes
Upload completed, will now verify:
Verifying:    2957312/2957312 bytes
Firmware updated/verified successfully!
```

## JTAG

The connector pad is named ``J201`` a 12 ways, 2 rows with 2mm pitch [board to board connector](http://it.rs-online.com/web/p/file-di-contatti-da-pcb/7656269/)
to be used with an [IDC connector](http://it.rs-online.com/web/p/products/714-2448/).

In order to use the JTAG maybe we have to use the ``buspirate_pullup`` configurations.

Name signal	|Pin out	| Pin out	| Name signal
------------|--------|---------|------------
nTRST	|1	|2	|GND
TDI	|3	|4	|GND
TDO	|5	|6	|GND
TMS	|7	|8	|GND
TCK	|9	|10	|GND
nSRST	|11	|12	|n/a

```
$ openocd -f netgear.cfg
```

```
$ telnet 4444
> targets
    TargetName         Type       Endian TapName            State       
--  ------------------ ---------- ------ ------------------ ------------
 0* bcm6348.cpu        mips_m4k   big    bcm6348.cpu        running
> halt
target state: halted
target halted in MIPS32 mode due to debug-request, pc: 0x8004462c
> mdw 0x8004462C 0x100
0x8004462c: 8fb20018 8fb10014 8fb00010 8c590008 03200008 27bd0020 27bdffe0 afb00010 
0x8004464c: 00a08021 afbf001c afb20018 afb10014 00809021 0c011161 00c08821 8e020028 
0x8004466c: 02402021 02002821 02203021 8fbf001c 8fb20018 8fb10014 8fb00010 8c590004 
0x8004468c: 03200008 27bd0020 27bdffc0 afbe0038 afb40028 afb00018 afbf003c afb70034 
0x800446ac: afb60030 afb5002c afb30024 afb20020 afb1001c 8f830014 3c0207ff 3442ff00 
0x800446cc: 00808021 00c0a021 00e0f021 00621024 00028036 04a30007 3c12bfff 8e1700c4 
0x800446ec: 8e150094 0017bf02 32f70001 080111ca 02a09021 3652ffff 00b29024 0005bf82 
0x8004470c: 2e420004 32f70001 14400004 2415ffff 24020005 164200c7 2403ffea 8e820000 
0x8004472c: 044000c4 2403ffea 28440064 108000c1 2656ffff 2ed60002 0002102b 16c200be 
0x8004474c: 8fbf003c 13c0002e 00000000 0c00ace3 24040017 144000c4 00000000 52c00011 
0x8004476c: 8e020094 8e020354 8e030094 8c4201d0 52430004 8e830000 104000ae 2403ffff 
0x8004478c: 8e830000 8e040024 0083202b 50800005 8e020094 0043102b 144000a6 2403ffff 
0x800447ac: 8e020094 24030005 14430008 00000000 12420006 02002021 8e05001c 0c0110db 
0x800447cc: 24a5ff88 1040009b 2403ffff 0c011085 02002021 10400097 2403ffff 8e0200c4 
0x800447ec: 3c041000 00821024 1040009f 00000000 16e0009d 8fbf003c 08011292 00601021 
0x8004480c: 40116000 3621001f 3821001f 40816000 00000040 00000040 00000040 000000c0 
0x8004482c: 3c13802d 2662cd98 8c4203dc 1602000d 32230001 40016000 32310001 3421001f 
0x8004484c: 3821001f 02218825 40916000 00000040 00000040 00000040 000000c0 08011290 
0x8004486c: 2403ffea 8e020094 56420015 2403ffff 12c00005 00000000 8e850000 8e040024 
0x8004488c: 54a4000f 2403ffff 40016000 32310001 3421001f 3821001f 02218825 40916000 
0x800448ac: 00000040 00000040 00000040 000000c0 54600060 00001821 08011291 8fbf003c 
0x800448cc: 12a3000f 2664cd98 52a2000e 8c9e03d4 40016000 32310001 3421001f 3821001f 
0x800448ec: 02218825 40916000 00000040 00000040 00000040 000000c0 080111bb 8e1700c4 
0x8004490c: 8c9e03d4 8e020014 021ef026 afa20010 10400004 2fde0001 02002821 0c01117d 
0x8004492c: 00003021 53c00007 8e0300c4 8e020028 2664cd98 8c42001c 0040f809 02002821 
0x8004494c: 8e0300c4 3c02efff 3442ffff 0017bf00 00621024 0057b825 ae1700c4 8e860000 
0x8004496c: 02002021 02402821 8e160018 0c0110bd 8e150028 13c00006 8fa20010 8e020028 
0x8004498c: 8c420020 0040f809 2664cd98 8fa20010 50400006 8e020028 2664cd98 02002821 
0x800449ac: 0c011191 00003021 8e020028 52a2000e 8e020018 8ea2002c 50400005 8e020028 
0x800449cc: 2664cd98 0040f809 02002821 8e020028 2664cd98 8c420030 0040f809 02002821 
0x800449ec: 08011283 00000000 12c20005 2664cd98 8ea20034 02002821 0040f809 02c03021 
0x80044a0c: 40016000 32310001 3421001f 3821001f 02218825 40916000 00000040 00000040
```

Links
-----

 - http://wiki.openwrt.org/toh/netgear/dg834.v4#installation
 - http://hexblog.com/files/recon%202010%20Skochinsky.pdf
 - http://www.ece.gatech.edu/academic/courses/ece2035/readings/embedded/MIPSvsARM.pdf
 - https://jaforeck.wordpress.com/2012/08/09/bricked-ebook-reader-bus-pirate-openocd-working-ebook-reader-14/
 - http://alvarop.com/2013/02/debugging-arm-cortex-m3-devices-with-gdb-and-openocd/
 - http://dangerousprototypes.com/docs/Gonemad%27s_Bus_Pirate/OpenOCD_walk_through
 - http://dangerousprototypes.com/forum/viewtopic.php?t=295
 - https://tux4u.wordpress.com/tag/bus-pirate/
