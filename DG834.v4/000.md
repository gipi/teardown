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

In order to use the JTAG maybe we have to use the ``buspirate_pullup`` configurations.