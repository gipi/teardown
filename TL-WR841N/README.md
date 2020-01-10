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

at offset ``0x3A10`` there is the ``LZMA`` compressed ``u-boot`` image (architecture ``MIPS`` 32bit big-endian, variant **micro**, at least, that one gives
the best result in Ghidra)

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

## UART

```
VCC GND RX TX
```

probably for some electronics-related reason, the input to the
serial is not taken with normal USB to serial converter: I used
BusPirate with the following configuration

```
Select output type:
 1. Open drain (H=Hi-Z, L=GND)
 2. Normal (H=3.3V, L=GND)

(1)>2
```

at the end is possible to access the serial with the credential

```
admin:sohoadmin
```

Instead ``u-boot`` is not accessible with this configuration, I don't know why.

## System info

```
# cat /proc/cpuinfo 
system type             : QCA953x
processor               : 0
cpu model               : MIPS 24Kc V7.4
BogoMIPS                : 433.15
wait instruction        : yes
microsecond timers      : yes
tlb_entries             : 16
extra interrupt vector  : yes
hardware watchpoint     : yes, count: 4, address/irw mask: [0x0000, 0x0050, 0x0890, 0x0dc8]
ASEs implemented        : mips16
shadow register sets    : 1
core                    : 0
VCED exceptions         : not available
VCEI exceptions         : not available
# ps
  PID  Uid     VmSize Stat Command
    1 root        400 S   init
    2 root            SW< [kthreadd]
    3 root            SW< [ksoftirqd/0]
    4 root            SW< [events/0]
    5 root            SW< [khelper]
    8 root            SW< [async/mgr]
   35 root            SW< [kblockd/0]
   55 root            SW  [pdflush]
   56 root            SW  [pdflush]
   57 root            SW< [kswapd0]
   74 root            SW< [mtdblockd]
  189 root       2636 S   /usr/bin/httpd
  197 root       2636 S   /usr/bin/httpd
  198 root       2636 S   /usr/bin/httpd
  204 root        340 S   syslogd -C -l 7
  208 root        300 S   klogd
  363 root        352 S   /sbin/udhcpc -h TL-WR841N -i eth1 -p /tmp/wr841n/udhc
  364 root        220 S   /sbin/udhcpc -h TL-WR841N -i eth1 -p /tmp/wr841n/udhc
  370 root        324 S   /usr/sbin/udhcpd /tmp/wr841n/udhcpd.conf
  422 root       2636 S   /usr/bin/httpd
  445 root       2636 S   /usr/bin/httpd
  446 root       2636 S   /usr/bin/httpd
  586 root       2636 S   /usr/bin/httpd
  588 root       2636 S   /usr/bin/httpd
  589 root       2636 S   /usr/bin/httpd
  590 root       2636 S   /usr/bin/httpd
  592 root       2636 S   /usr/bin/httpd
  595 root       2636 S   /usr/bin/httpd
  609 root        652 S   hostapd -B /tmp/topology.conf
  610 root       2636 S   /usr/bin/httpd
  619 root       2636 S   /usr/bin/httpd
  620 root       2636 S   /usr/bin/httpd
  621 root       2636 S   /usr/bin/httpd
  624 root       2636 S   /usr/bin/httpd
  625 root       2636 S   /usr/bin/httpd
  627 root       2636 S   /usr/bin/httpd
  630 root       2636 S   /usr/bin/httpd
  634 root        300 S   /usr/bin/lld2d br0 ath0
  646 root        344 S   /usr/bin/dropbear -p 22 -r /tmp/dropbear/dropbear_rsa
  659 root        532 R   -sh
  668 root        400 R   ps
```

## Boot log

```
U-Boot 1.1.4 (Mar 10 2015 - 15:00:34)

ap143-2.0 - Honey Bee 2.0

DRAM:  32 MB
Flash Manuf Id 0xc8, DeviceId0 0x40, DeviceId1 0x16
flash size 4MB, sector count = 64
Flash:  4 MB
Using default environment

In:    serial
Out:   serial
Err:   serial
Net:   ath_gmac_enet_initialize...
ath_gmac_enet_initialize: reset mask:c02200
Scorpion ---->S27 PHY*
S27 reg init
: cfg1 0x800c0000 cfg2 0x7114
eth0: ba:be:fa:ce:08:41
athrs27_phy_setup ATHR_PHY_CONTROL 4 :1000
athrs27_phy_setup ATHR_PHY_SPEC_STAUS 4 :10
eth0 up
Honey Bee ---->  MAC 1 S27 PHY *
S27 reg init
ATHRS27: resetting s27
ATHRS27: s27 reset done
: cfg1 0x800c0000 cfg2 0x7214
eth1: ba:be:fa:ce:08:41
athrs27_phy_setup ATHR_PHY_CONTROL 0 :1000
athrs27_phy_setup ATHR_PHY_SPEC_STAUS 0 :10
athrs27_phy_setup ATHR_PHY_CONTROL 1 :1000
athrs27_phy_setup ATHR_PHY_SPEC_STAUS 1 :10
athrs27_phy_setup ATHR_PHY_CONTROL 2 :1000
athrs27_phy_setup ATHR_PHY_SPEC_STAUS 2 :10
athrs27_phy_setup ATHR_PHY_CONTROL 3 :1000
athrs27_phy_setup ATHR_PHY_SPEC_STAUS 3 :10
eth1 up
eth0, eth1
Setting 0x181162c0 to 0x58b1a100
is_auto_upload_firmware=0
Autobooting in 1 seconds
## Booting image at 9f020000 ...
   Uncompressing Kernel Image ... OK

Starting kernel ...

Booting QCA953x
Linux version 2.6.31 (tomcat@buildserver) (gcc version 4.3.3 (GCC) ) #35 Tue Mar 10 15:10:01 CST 2015
Ram size passed from bootloader =32M
flash_size passed from bootloader = 4
CPU revision is: 00019374 (MIPS 24Kc)
ath_sys_frequency: cpu apb ddr apb cpu 650 ddr 391 ahb 216
Determined physical RAM map:
 memory: 02000000 @ 00000000 (usable)
Zone PFN ranges:
  Normal   0x00000000 -> 0x00002000
Movable zone start PFN for each node
early_node_map[1] active PFN ranges
    0: 0x00000000 -> 0x00002000
Built 1 zonelists in Zone order, mobility grouping on.  Total pages: 8128
Kernel command line: console=ttyS0,115200 root=31:2 rootfstype=squashfs init=/sbin/init mtdparts=ath-nor0:128k(u-boot),1024k(kernel),2816k(rootfs),64k(config),64k(art) mem=32M
PID hash table entries: 128 (order: 7, 512 bytes)
Dentry cache hash table entries: 4096 (order: 2, 16384 bytes)
Inode-cache hash table entries: 2048 (order: 1, 8192 bytes)
Primary instruction cache 64kB, VIPT, 4-way, linesize 32 bytes.
Primary data cache 32kB, 4-way, VIPT, cache aliases, linesize 32 bytes
Writing ErrCtl register=00000000
Readback ErrCtl register=00000000
Memory: 24068k/32768k available (1638k kernel code, 8700k reserved, 415k data, 112k init, 0k highmem)
NR_IRQS:128
plat_time_init: plat time init done
Calibrating delay loop... 433.15 BogoMIPS (lpj=866304)
Mount-cache hash table entries: 512

****************ALLOC***********************
 Packet mem: 80231420 (0x600000 bytes)
********************************************

NET: Registered protocol family 16
ath_pcibios_init: bus 0
***** Warning PCIe 0 H/W not found !!!
registering PCI controller with io_map_base unset
bio: create slab <bio-0> at 0
NET: Registered protocol family 2
IP route cache hash table entries: 1024 (order: 0, 4096 bytes)
net_link: create socket ok.
TCP established hash table entries: 1024 (order: 1, 8192 bytes)
TCP bind hash table entries: 1024 (order: 0, 4096 bytes)
TCP: Hash tables configured (established 1024 bind 1024)
TCP reno registered
NET: Registered protocol family 1
ATH GPIOC major 0
squashfs: version 4.0 (2009/01/31) Phillip Lougher
msgmni has been set to 47
io scheduler noop registered
io scheduler deadline registered (default)
Serial: 8250/16550 driver, 1 ports, IRQ sharing disabled
serial8250.0: ttyS0 at MMIO 0xb8020000 (irq = 19) is a 16550A
console [ttyS0] enabled
PPP generic driver version 2.4.2
NET: Registered protocol family 24
5 cmdlinepart partitions found on MTD device ath-nor0
Creating 5 MTD partitions on "ath-nor0":
0x000000000000-0x000000020000 : "u-boot"
0x000000020000-0x000000120000 : "kernel"
0x000000120000-0x0000003e0000 : "rootfs"
0x0000003e0000-0x0000003f0000 : "config"
0x0000003f0000-0x000000400000 : "art"
->Oops: flash id 0xc84016 .
Ooops, why the devices couldn't been initialed?
TCP cubic registered
NET: Registered protocol family 17
802.1Q VLAN Support v1.8 Ben Greear <greearb@candelatech.com>
All bugs added by David S. Miller <davem@redhat.com>
athwdt_init: Registering WDT success
VFS: Mounted root (squashfs filesystem) readonly on device 31:2.
Freeing unused kernel memory: 112k freed
^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@init started:  BusyBox v1.01 (2015.03.10-07:17+0000) multi-call binary
This Board use 2.6.31
^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@xt_time: kernel timezone is -0000
^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@nf_conntrack version 0.5.0 (512 buckets, 5120 max)
^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@ip_tables: (C) 2000-2006 Netfilter Core Team
insmod: cannot open module `/lib/modules/2.6.31/kernel/iptable_raw.ko': No such file or directory
^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@insmod: cannot open module `/lib/modules/2.6.31/kernel/flashid.ko': No such file or directory
PPPoL2TP kernel driver, V1.0
PPTP driver version 0.8.3
insmod: cannot open module `/lib/modules/2.6.31/kernel/harmony.ko': No such file or directory
insmod: cannot open module `/lib/modules/2.6.31/kernel/af_key.ko': No such file or directory
insmod: cannot open module `/lib/modules/2.6.31/kernel/xfrm_user.ko': No such file or directory
^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@Now flash open!
Now flash open!
qca955x_GMAC: Length per segment 1536
953x_GMAC: qca953x_gmac_attach
Link Int Enabled
qca953x_set_gmac_caps  CHECK DMA STATUS
mac:1 Registering S27....
qca955x_GMAC: RX TASKLET - Pkts per Intr:18
qca955x_GMAC: Max segments per packet :   1
qca955x_GMAC: Max tx descriptor count :   511
qca955x_GMAC: Max rx descriptor count :   128
qca955x_GMAC: Mac capability flags    :   2D81
953x_GMAC: qca953x_gmac_attach
Link Int Enabled
qca953x_set_gmac_caps  CHECK DMA STATUS
mac:0 Registering S27....
qca955x_GMAC: RX TASKLET - Pkts per Intr:18
qca955x_GMAC: Max segments per packet :   1
qca955x_GMAC: Max tx descriptor count :   511
qca955x_GMAC: Max rx descriptor count :   128
qca955x_GMAC: Mac capability flags    :   2581

 (none) mips #35 Tue Mar 10 15:10:01 CST 2015 (none)
(none) login: athr_gmac_ring_alloc Allocated 8176 at 0x81c60000
athr_gmac_ring_alloc Allocated 2048 at 0x81cbc000
HONEYBEE ----> S27 PHY MDIO
ATHRS27: resetting s27
ATHRS27: s27 reset done
++++ athrs27_igmp_setup once
port0 vid is 0xb000b
port1 vid is 0x30003
port2 vid is 0x50005
port3 vid is 0x70007
port4 vid is 0x90009
++ PVID: 0x0000000b, bitmap: 0x0000001f
++ PVID: 0x00000003, bitmap: 0x0000001f
++ PVID: 0x00000005, bitmap: 0x0000001f
++ PVID: 0x00000007, bitmap: 0x0000001f
++ PVID: 0x00000009, bitmap: 0x0000001f
vtable vid: 0x00000002, bitmap 0x00000003
vtable vid: 0x00000004, bitmap 0x00000005
vtable vid: 0x00000006, bitmap 0x00000007
vtable vid: 0x00000008, bitmap 0x00000009
vtable vid: 0x0000000a, bitmap 0x0000000b
vtable vid: 0x0000000c, bitmap 0x0000000d
vtable vid: 0x0000000e, bitmap 0x0000000f
vtable vid: 0x00000010, bitmap 0x00000011
vtable vid: 0x00000012, bitmap 0x00000013
vtable vid: 0x00000014, bitmap 0x00000015
vtable vid: 0x00000016, bitmap 0x00000017
vtable vid: 0x00000018, bitmap 0x00000019
vtable vid: 0x0000001a, bitmap 0x0000001b
vtable vid: 0x0000001c, bitmap 0x0000001d
vtable vid: 0x0000001e, bitmap 0x0000001f
vtable vid: 0x00000020, bitmap 0x00000021
Setting Drop CRC Errors, Pause Frames and Length Error frames
Setting PHY...
athr_gmac_ring_alloc Allocated 8176 at 0x81cdc000
athr_gmac_ring_alloc Allocated 2048 at 0x81e99000
HONEYBEE ----> S27 PHY MDIO
Setting Drop CRC Errors, Pause Frames and Length Error frames
Setting PHY...
device eth0 entered promiscuous mode
Now flash open!
athr_gmac_ring_free Freeing at 0x81cdc000
athr_gmac_ring_free Freeing at 0x81e99000
athr_gmac_ring_alloc Allocated 8176 at 0x81c52000
athr_gmac_ring_alloc Allocated 2048 at 0x81e99000
```

## Emulation

With ``qemu-mips-static`` is possible to run the ``httpd`` daemon, take in mind
that versions previous to 4.2 have problems to run it, so in Debian I'm using
the version installed in the host

```
$ docker run -it \
    -v $PWD/TL-WR841N/extra/_wr841nv10_wr841ndv10_en_3_16_9_up_boot\(150310\).bin.extracted/squashfs-root/:/tp-link \
    -v /usr/bin/qemu-mips-static:/usr/bin/qemu-mips-static  \
    ubuntu:19.10 /bin/bash
root@3ca21c357fdc:/# qemu-mips-static -L /tp-link/ /tp-link/usr/bin/httpd -h
/tp-link/usr/bin/httpd: cache '/etc/ld.so.cache' is corrupt
Usage:  /tp-link/usr/bin/httpd [-krfhv] [--help] [--version]
        [-k, --kill]    kill all httpd threads
        [-r, --reset]   start httpd, and reset all settings to default
        [-f, --freeup]  this parameter allows httpd can update any firmware.
        [-h, --help]    help
        [-v, --version] version
root@3ca21c357fdc:/# qemu-mips-static -L /tp-link/ /tp-link/usr/bin/httpd 
/tp-link/usr/bin/httpd: cache '/etc/ld.so.cache' is corrupt
Segmentation fault (core dumped)
root@3ca21c357fdc:/# chroot /tp-link/ bin/sh


BusyBox v1.01 (2015.03.10-07:17+0000) Built-in shell (msh)
Enter 'help' for a list of built-in commands.

# httpd 
SIGSEGV - core dumped
$ qemu-system-mips \
    -M malta \
    -kernel mips/vmlinux-2.6.32-5-4kc-malta \
    -hda mips/debian_squeeze_mips_standard.qcow2 \
    -append "root=/dev/sda1 console=ttyS0" \
    -nographic \
    -serial mon:stdio \
    -nic user,hostfwd=tcp::2222-:22
```

In this last case you have an old Debian system that needs the following repositories
enabled ([source](https://wiki.debian.org/it/DebianSqueeze))

```
deb http://archive.debian.org/debian squeeze main
deb http://archive.debian.org/debian squeeze-lts main
```

with the following in ``/etc/apt/apt.conf``

```
Acquire::Check-Valid-Until false;
```

## Links

 - [Product page](https://www.tp-link.com/it/support/download/tl-wr841n/v10/)
 - [OpenWRT page](https://openwrt.org/toh/tp-link/tl-wr841nd)
 - [TP Link header](https://oldwiki.archive.openwrt.org/doc/techref/header)
 - [CVEs](https://www.cvedetails.com/vulnerability-list.php?vendor_id=11936&product_id=23482&version_id=0&page=1&hasexp=0&opdos=0&opec=0&opov=0&opcsrf=0&opgpriv=0&opsqli=0&opxss=0&opdirt=0&opmemc=0&ophttprs=0&opbyp=0&opfileinc=0&opginf=0&cvssscoremin=1&cvssscoremax=0&year=0&month=0&cweid=0&order=1&trc=3&sha=3bd1591ed25ebc92cae58c9f45417e81f43a35dd)
 - [Hardware Reversing with the TP-Link TL-WR841N Router - Part 2](https://www.zerodayinitiative.com/blog/2019/12/2/mindshare-hardware-reversing-with-the-tp-link-tl-wr841n-router-part-2) another revision with a vulnerability
