# Kobo Touch N905

N905J36205687

| Component | Description |
|-----------|-------------|
| core | MCIMX50 |
| architecture | ARM Cortex-A8 |
| Display power supply | TPS65185 [product page](https://www.ti.com/product/TPS65185) |
| Optical touch controller | NN1001 |

 - https://www.nxp.com/docs/en/data-sheet/IMX50CEC.pdf
 - https://misc.andi.de1.cc/kobo/
 - http://naberius.de/2013/06/19/collected-exploring-the-tolino-shine/
 - [Specifications for kobo readers](https://en.wikipedia.org/wiki/Kobo_eReader)
 - [u-boot-fslc](https://github.com/akemnade/u-boot-fslc/wiki)
 - [mfgtools](https://github.com/NXPmicro/mfgtools) by ``NXP`` I.MX Chip image deploy tools
 - [Free/libre software for Kobo ebook readers](https://github.com/lgeek/okreader)

## U-Boot

```
$ sudo dd if=u-boot-2009.08/u-boot_mddr_256-E50610-K4X2G323PC.bin of=/dev/sdd
```

It's possible to remove the watchdog that powers down the board doing a ``i2c probe`` just after
accessing the ``u-boot`` console ([source](https://github.com/twhitehead/notes-kobohacking)).

``u-boot`` has a special command to load the kernel: ``load_ntxkernel`` that expects it
at a special offset (``0x100000``) from the start of the sd card, following
these values as found in ``board/freescale/mx50_rdp/ntx_comm.c``

```c
// binaries sector number of sd card .
#define SD_OFFSET_SECS_KERNEL		2048
#define SD_OFFSET_SECS_INITRD		12288
#define SD_OFFSET_SECS_INITRD2		8192
#define SD_OFFSET_SECS_HWCFG		1024
#define SD_OFFSET_SECS_LOGO			18432
#define SD_OFFSET_SECS_WAVEFORM		14336

#define SD_OFFSET_SECS_SN		1

#define DEFAULT_LOAD_KERNEL_SZ	8192
#define DEFAULT_LOAD_RD_SZ	8192
#define KERNEL_RAM_ADDR		CONFIG_LOADADDR

```

## Kernel

You can use the dump from kobolabs

```
$ make ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabi- uImage
$ sudo dd if=/hack/kobo/linux-2.6.35.3/arch/arm/boot/uImage of=/dev/sdd seek=2048 && sync
```

or adapt the official linux source code

```
$ make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- imx_v6_v7_defconfig
$ make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- LOADADDR=0x70800000  uImage -j 5
$ make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- dtbs
```

## Serial

Connecting to the serial port (four exposed pins at the corner of the board)
is possible to access a terminal (username ``root`` without password):

```
[root@kobo ~]# cat /proc/cpuinfo 
Processor       : ARMv7 Processor rev 5 (v7l)
BogoMIPS        : 799.53
Features        : swp half thumb fastmult vfp edsp neon vfpv3 
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x2
CPU part        : 0xc08
CPU revision    : 5

Hardware        : Freescale MX50 Reference Design Platform
Revision        : 50011
Serial          : 0000000000000000
[root@kobo ~]# lsmod

dhd 137565 0 - Live 0x7f00c000
sdio_wifi_pwr 490 0 - Live 0x7f006000
lowmem 936 0 - Live 0x7f000000 (P)
[root@kobo ~]# cat /proc/modules
dhd 137565 0 - Live 0x7f00c000
sdio_wifi_pwr 490 0 - Live 0x7f006000
lowmem 936 0 - Live 0x7f000000 (P)
[root@kobo ~]# ps afx
PID   USER     TIME  COMMAND
    1 root      0:00 init
    2 root      0:00 [kthreadd]
    3 root      0:00 [ksoftirqd/0]
    4 root      0:01 [events/0]
    5 root      0:00 [khelper]
    8 root      0:00 [async/mgr]
    9 root      0:00 [pm]
   62 root      0:00 [usb_wakeup thre]
   63 root      0:00 [usb_wakeup thre]
  114 root      0:00 [sync_supers]
  116 root      0:00 [bdi-default]
  118 root      0:00 [kblockd/0]
  130 root      0:00 [mxc_spi.2]
  138 root      0:00 [khubd]
  153 root      0:00 [kmmcd]
  209 root      0:00 [swapper]
  213 root      0:00 [zq_calib]
  219 root      0:00 [kswapd0]
  266 root      0:00 [aio/0]
  276 root      0:00 [crypto/0]
  285 root      0:00 [submit/0]
  288 root      0:00 [tps65185_PWRGOO]
  290 root      0:00 [tps65185_INT/0]
  375 root      0:00 [kapmd]
  421 root      0:00 [kconservative/0]
  425 root      0:00 [mxc_chg]
  426 root      0:00 [pmic_battery.1]
  430 root      0:00 [esdhc_wq/0]
  432 root      0:00 [esdhc_wq/0]
  434 root      0:01 [esdhc_wq/0]
  454 root      0:36 [mmcqd]
  460 root      0:00 [jbd2/mmcblk0p1-]
  461 root      0:00 [ext4-dio-unwrit]
  465 root      0:00 [submit/0]
  468 root      0:00 [flush-179:0]
  489 root      0:00 /sbin/udevd -d
  570 messageb  0:00 /bin/dbus-daemon --system
  571 root      0:00 /bin/dbus-daemon --session --print-address --fork
  572 root      0:00 /usr/local/Kobo/hindenburg
  573 root      0:39 /usr/local/Kobo/nickel -platform kobo -skipFontLoad
  576 root      0:00 {kobo_getty.sh} /bin/sh /sbin/kobo_getty.sh
  579 root      0:00 -sh
  884 root      0:00 syslogd -C500 -S
  887 root      0:00 {sickel-launcher} /bin/sh /usr/local/Kobo/sickel-launcher.
  888 root      0:00 /usr/local/Kobo/sickel -platform kobo:noscreen
  961 root      0:00 [iscan_sysioc]
  962 root      0:00 [dhd_watchdog]
  963 root      0:00 [dhd_dpc]
  964 root      0:00 [dhd_sysioc]
  965 root      0:04 [ksdioirqd/mmc2]
  979 root      0:00 wpa_supplicant -D wext -s -i eth0 -c /etc/wpa_supplicant/w
  983 root      0:00 /libexec/dhcpcd-dbus
  993 root      0:00 dhcpcd -d -z eth0
 1035 root      0:00 ps afx
[root@kobo ~]# ls /dev/
apm_bios            rtc0                tty44
block               tty                 tty45
bus                 tty0                tty46
char                tty1                tty47
console             tty10               tty48
cpu_dma_latency     tty11               tty49
crypto              tty12               tty5
fb0                 tty13               tty50
full                tty14               tty51
i2c-0               tty15               tty52
i2c-1               tty16               tty53
i2c-2               tty17               tty54
input               tty18               tty55
kmem                tty19               tty56
kmsg                tty2                tty57
log                 tty20               tty58
loop0               tty21               tty59
loop1               tty22               tty6
loop2               tty23               tty60
loop3               tty24               tty61
loop4               tty25               tty62
loop5               tty26               tty63
loop6               tty27               tty7
loop7               tty28               tty8
mem                 tty29               tty9
mmcblk0             tty3                ttymxc0
mmcblk0p1           tty30               ttymxc1
mmcblk0p2           tty31               ttymxc2
mmcblk0p3           tty32               ttyp0
mxs_viim            tty33               ttyp1
network_latency     tty34               ttyp2
network_throughput  tty35               ttyp3
ntx_io              tty36               urandom
null                tty37               usbdev1.1
ptmx                tty38               vcs
ptyp0               tty39               vcs1
ptyp1               tty4                vcsa
ptyp2               tty40               vcsa1
ptyp3               tty41               zero
pxp_device          tty42
random              tty43
[root@kobo ~]# cat /proc/partitions 
major minor  #blocks  name

 179        0    1934336 mmcblk0
 179        1     262144 mmcblk0p1
 179        2     262144 mmcblk0p2
 179        3    1397247 mmcblk0p3

[root@kobo ~]# mount
rootfs on / type rootfs (rw)
/dev/root on / type ext4 (rw,noatime,nodiratime,barrier=1,data=ordered)
none on /proc type proc (rw,relatime)
none on /tmp type tmpfs (rw,relatime,size=16384k)
none on /dev type tmpfs (rw,relatime)
none on /var/lib type tmpfs (rw,relatime,size=16k)
none on /var/log type tmpfs (rw,relatime,size=16k)
none on /var/run type tmpfs (rw,relatime,size=128k)
none on /sys type sysfs (rw,relatime)
/dev/mmcblk0p3 on /mnt/onboard type vfat (rw,noatime,nodiratime,fmask=0022,dmask=0022,codepage=cp437,iocharset=iso8859-1,shortname=mixed,utf8,errors=remount-ro)
```
