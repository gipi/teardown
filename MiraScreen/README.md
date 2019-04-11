# MiraScreen

It's a wireless display that mimic the ``ChromeCast`` but without
possibility to install applications. [This](https://www.mirascreen.com/prod?p_id=3)
is the product page.

There are some [vulnerabilities reported](https://blog.checkpoint.com/wp-content/uploads/2015/12/EZCast_Report_Check_Point.pdf).

## RLT8188ESU

Use this chip for the WiFi: it's connected via ``USB``

 - [Datasheet](http://www.evselectro.com/image/data/datasheet/RTL8188EUS.pdf)

## AM8252

It's the main processor on this device.

## Download mode

As indicated in this [page](http://www.iezvu.com/download_tool.php?&l=en) it's possible to
activate the flashing mode shorting pins 19 and 20 (right side, bottom to up) each other and then apply power.

From some hints (like the update program library) I think the device is going to use the ``ADFU`` (Actions Device Firmware Update) protocol like [this](https://github.com/96boards-bubblegum/linaro-adfu-tool) but further analysis make it less sure: that is from **Actions Semiconductors** not **Actions Microeletronics**, BTW [this an old page](https://web.archive.org/web/20160418032428/http://wiki.s1mp3.org/USB_modes/)
describing the protocol that is very similar to what I'm seeing.

After that the device is seen as 

```
Bus 003 Device 010: ID 1de1:1205 Actions Microelectronics Co.
Device Descriptor:
  bLength                18
  bDescriptorType         1
  bcdUSB               2.00
  bDeviceClass            0 (Defined at Interface level)
  bDeviceSubClass         0
  bDeviceProtocol         0
  bMaxPacketSize0        64
  idVendor           0x1de1 Actions Microelectronics Co.
  idProduct          0x1205
  bcdDevice            1.00
  iManufacturer           0
  iProduct                0
  iSerial                 0
  bNumConfigurations      1
  Configuration Descriptor:
    bLength                 9
    bDescriptorType         2
    wTotalLength           32
    bNumInterfaces          1
    bConfigurationValue     1
    iConfiguration          0 
    bmAttributes         0x80
      (Bus Powered)
    MaxPower              300mA
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        0
      bAlternateSetting       0
      bNumEndpoints           2
      bInterfaceClass       255 Vendor Specific Class
      bInterfaceSubClass    255 Vendor Specific Subclass
      bInterfaceProtocol    255 Vendor Specific Protocol
      iInterface              0 
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x01  EP 1 OUT
        bmAttributes            2
          Transfer Type            Bulk
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0040  1x 64 bytes
        bInterval               0
Device Qualifier (for other device speed):
  bLength                10
  bDescriptorType         6
  bcdUSB               2.00
  bDeviceClass            0 (Defined at Interface level)
  bDeviceSubClass         0
  bDeviceProtocol         0
  bMaxPacketSize0        64
  bNumConfigurations      1
Device Status:     0x0000
  (Bus Powered)
```

same vendor id and product id referenced into the [Forensic analysis of Chromecast and
Miracast devices](https://www.os3.nl/_media/2013-2014/courses/ccf/chromecast-cedric-peter.pdf)

Also [here](https://ao2.it/en/blog/2012/01/18/usb-projectors-linux-and-libam7xxx) are talking
about this family of chip and the possibility to access as mass storage devices: also exists
a project called [libam7xxx](https://git.ao2.it/libam7xxx.git/) for it.

Probably the update process happens using a ``SCSI`` over ``USB`` protocol

## Serial

It's possible to reach an ``UART`` interface soldering some wires to the two
pads at the bottom of the board (see image below).

![](UART.jpg)

This gives us a [boot log](mirascreen_boot.log) and a root shell to inspect the system

```
# uname -ra
Linux MiraScreen D281A14F 2.6.27.29 #7 PREEMPT Fri Apr 1 14:28:32 CST 2016 mips GNU/Linux
# cat /proc/cpuinfo 
system type             : MIPS AM7X
processor               : 0
cpu model               : MIPS 24K V4.12
BogoMIPS                : 364.54
wait instruction        : yes
microsecond timers      : yes
tlb_entries             : 32
extra interrupt vector  : yes
hardware watchpoint     : yes
ASEs implemented        : mips16 dsp
shadow register sets    : 2
core                    : 0
VCED exceptions         : not available
VCEI exceptions         : not available
# cat /proc/partitions

major minor  #blocks  name

  93     0     111616 nand_block
  93     1      65536 nand_block1
  93     2      10240 nand_block2
  93     3       1024 nand_block3
  93     4      32768 nand_block4
# mount
rootfs on / type rootfs (rw)
/dev/nftla1 on / type ext2 (rw,noatime,nodiratime,errors=continue,user_xattr,acl)
dev on /dev type ramfs (rw)
yan on /proc type proc (rw)
tmpfs on /tmp type tmpfs (rw)
sysfs on /sys type sysfs (rw)
none on /proc/bus/usb type usbfs (rw)
/dev/partitions/vram on /mnt/vram type vfat (rw,noatime,nodiratime,fmask=0022,dmask=0022,codepage=cp437,iocharset=utf8,shortname=mixed)
/dev/partitions/user1 on /mnt/user1 type ext2 (rw,sync,noatime,nodiratime,errors=continue,user_xattr,acl)
# cat /proc/filesystems 
nodev   sysfs
nodev   rootfs
nodev   bdev
nodev   proc
nodev   sockfs
nodev   usbfs
nodev   pipefs
nodev   tmpfs
        ext2
nodev   ramfs
        vfat
        ntfs
        exfat
        romfs
# ls -al /dev/miracast

drwxr-xr-x    3 root     root            0 Jan  1 00:00 .
drwxr-xr-x   18 100013   100001       1024 Jan  1 00:00 ..
crw-r--r--    1 root     root      14,   0 Apr  1  2016 DAC
crw-rw----    1 root     root     247,   0 Jan  1 00:00 am7x-cec
crw-rw----    1 root     root     248,   0 Jan  1 00:00 am7x-cipher
crw-rw----    1 root     root     250,   0 Jan  1 00:00 amreg
crw-rw----    1 root     root       5,   1 Jan  1 00:01 console
crw-rw----    1 root     root      10,  63 Jan  1 00:00 cpu_dma_latency
brw-r--r--    1 root     root     249,   0 Apr  1  2016 emmc_block
brw-r--r--    1 root     root     249,   1 Apr  1  2016 emmc_block1
brw-r--r--    1 root     root     249,   2 Apr  1  2016 emmc_block2
crw-rw----    1 root     root      13,  64 Jan  1 00:00 event0
crw-rw----    1 root     root       1,   7 Jan  1 00:00 full
crw-r--r--    1 root     root     253,   0 Apr  1  2016 gpio
crw-rw-rw-    1 root     root      30,   0 Jan  1 00:00 graph
crw-rw----    1 root     root       1,  11 Jan  1 00:00 kmsg
crw-r--r--    1 root     root      28,   0 Apr  1  2016 lcm
brw-rw----    1 root     root       7,   0 Apr  1  2016 loop0
brw-rw----    1 root     root       7,   1 Jan  1 00:00 loop1
brw-rw----    1 root     root       7,   2 Jan  1 00:00 loop2
brw-rw----    1 root     root       7,   3 Jan  1 00:00 loop3
brw-rw----    1 root     root       7,   4 Jan  1 00:00 loop4
brw-rw----    1 root     root       7,   5 Jan  1 00:00 loop5
brw-rw----    1 root     root       7,   6 Jan  1 00:00 loop6
brw-rw----    1 root     root       7,   7 Jan  1 00:00 loop7
crw-rw----    1 root     root       1,   1 Apr  1  2016 mem
brw-rw----    1 root     root      93,   0 Jan  1 00:00 nand_block
brw-rw----    1 root     root      93,   1 Jan  1 00:00 nand_block1
brw-rw----    1 root     root      93,   2 Jan  1 00:00 nand_block2
brw-rw----    1 root     root      93,   3 Jan  1 00:00 nand_block3
brw-rw----    1 root     root      93,   4 Jan  1 00:00 nand_block4
crw-rw----    1 root     root      10,  62 Jan  1 00:00 network_latency
crw-rw----    1 root     root      10,  61 Jan  1 00:00 network_throughput
brw-r--r--    1 root     root      93,   0 Apr  1  2016 nftla
brw-r--r--    1 root     root      93,   1 Apr  1  2016 nftla1
brw-r--r--    1 root     root      93,   2 Apr  1  2016 nftla2
crw-rw----    1 root     root       1,   3 Apr  1  2016 null
drwxr-xr-x    2 100013   100001          0 Jan  1 00:00 partitions
crw-r--r--    1 root     root     251,   0 Apr  1  2016 pmu
brw-rw----    1 root     root       1,   0 Apr  1  2016 ram0
brw-r--r--    1 root     root       1,   1 Apr  1  2016 ram1
crw-rw----    1 root     root       1,   8 Jan  1 00:00 random
lrwxrwxrwx    1 root     root           11 Jan  1 00:00 root -> nand_block1
crw-rw----    1 root     root     254,   0 Apr  1  2016 rtc0
crw-rw----    1 root     root       1, 250 Apr  1  2016 sysbuf
crw-r--r--    1 root     root     252,   0 Apr  1  2016 sysmsg
crw-rw----    1 root     root       5,   0 Jan  1 00:00 tty
crw-rw----    1 root     root       4,  64 Apr  1  2016 ttyS0
crw-rw----    1 root     root       4,  65 Apr  1  2016 ttyS1
crw-r--r--    1 root     root     240,   0 Apr  1  2016 upgrade
crw-rw----    1 root     root       1,   9 Jan  1 00:00 urandom
crw-rw----    1 root     root       1,   5 Apr  1  2016 zero
# ls -al /dev/partitions/
drwxr-xr-x    2 100013   100001          0 Jan  1 00:00 .
drwxr-xr-x    3 root     root            0 Jan  1 00:00 ..
-rw-r--r--    1 100013   100001          0 Apr  1  2016 1.txt
brw-r--r--    1 root     root      93,   4 Jan  1 00:00 reserve
brw-r--r--    1 root     root      93,   1 Jan  1 00:00 rootfs
brw-r--r--    1 root     root      93,   2 Jan  1 00:00 user1
brw-r--r--    1 root     root      93,   3 Jan  1 00:00 vram
# df -h
Filesystem                Size      Used Available Use% Mounted on
/dev/nftla1              62.1M     56.6M      5.5M  91% /
/dev/partitions/vram   1001.5K      8.0K    993.5K   1% /mnt/vram
/dev/partitions/user1
                          9.7M      7.9M      1.7M  82% /mnt/user1
# free 
              total         used         free       shared      buffers
  Mem:        63536        31268        32268            0         3164
 Swap:            0            0            0
Total:        63536        31268        32268
```

Here I'm trying to make sense of the partitions

```
# dd if=/dev/nand_block | hexdump -C | head -n 20
blkdev_open
00000000  41 63 74 69 6f 6e 73 20  4d 69 63 72 6f 45 6c 65  |Actions MicroEle|
00000010  74 72 6f 6e 69 63 73 20  4e 61 74 69 76 65 20 4d  |tronics Native M|
00000020  42 52 20 66 6f 72 20 45  6d 62 65 64 64 65 64 20  |BR for Embedded |
00000030  4c 69 6e 75 78 20 6f 6e  20 4e 61 6e 64 46 6c 61  |Linux on NandFla|
00000040  73 68 0a 56 65 72 73 69  6f 6e 20 31 2e 30 2e 30  |sh.Version 1.0.0|
00000050  0a 31 37 3a 33 37 3a 34  30 0a 41 75 67 20 31 30  |.17:37:40.Aug 10|
00000060  20 32 30 31 35 0a 44 65  73 69 67 6e 65 64 20 62  | 2015.Designed b|
00000070  79 20 50 61 6e 20 52 75  6f 63 68 65 6e 20 3c 69  |y Pan Ruochen <i|
00000080  6a 6b 78 79 7a 40 6d 73  6e 2e 63 6f 6d 3e 0a 00  |jkxyz@msn.com>..|
00000090  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
000000f0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 83 72  |...............r|
00000100  6f 6f 74 66 73 00 80 00  00 00 00 00 02 00 83 75  |ootfs..........u|
00000110  73 65 72 31 00 00 80 00  02 00 00 50 00 00 0b 76  |ser1.......P...v|
00000120  72 61 6d 00 00 00 80 50  02 00 00 08 00 00 00 72  |ram....P.......r|
00000130  65 73 65 72 76 00 00 68  02 00 00 00 01 00 00 00  |eserv..h........|
00000140  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
000001f0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 4c 59  |..............LY|
00000200  ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff  |................|
```

To understand the ``nftl`` filesystem (?) there are some
links [1](http://etutorials.org/Linux+systems/embedded+linux+systems/Chapter+7.+Storage+Device+Manipulation/7.1+MTD-Supported+Devices/)
[2](http://www.tldp.org/HOWTO/Disk-on-Chip-HOWTO/intro.html)
that talk about that.

## Networking

It seems to expose two wireless interfaces.

It connects to the router's net only when put into ``Airplay`` mode.

## Default root password

```
# cat /etc/shadow 
root:$1$qsNdVNb0$U5HuLrYhZ6vbslIiyaQlA1:14610:0:99999:7:::
```

that corresponds to ``000000``.

## TODO

Write a client application to flash firmware, use this [project](https://github.com/rounaksingh/faltu)
as an example in writing a ``SCSI`` over ``USB`` app. There is a [question on SO about a similar thing](https://reverseengineering.stackexchange.com/questions/19602/scsi-interface-through-usb) that shows ``lsscsi``
command.

 - [Universal Serial Bus Mass Storage Class](http://www.usb.org/developers/docs/devclass_docs/usbmassbulk_10.pdf)
