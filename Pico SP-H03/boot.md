# Boot log

Connecting to the ``UART0`` at 115200 baud with the Bus Pirate
we obtain the following boot log

```
LED Driver init.
TSADC init.
MAX8903A: DC Input.
MAX8903:DC = 1
␛[1;34m DC Input Mode! ␛[0m 
NAND: V7018
[NAND        ] [PORT CONFIG - CS[0, 1] [NAND Data Port: GPIO_B Hw4 ~ Hw11]
[NAND        ] [BClk 156MHZ][1Tick 65][RE-S:0,P:7,H:4][WR-S:0,P:5,H:4][COM-S:2,P:15,H:15]
[NAND        ] [NB Area:4MB][DT Area:931MB][HD Area0:9MB][HD Area1:9MB]

=============================================
    tcboot ver 0.5 for TCC8900 Linux
    Board ver TCC8900_WINCE_LINUX_DEMO
    DATE:Jul 12 2010, TIME:13:11:27
=============================================
CHIP ID: 0x0000000E0DD3C3B0
CLOCK: PLL0:540, PLL1:560, PLL2:468, PLL3:528, CPU:506, BUS:280  
       DDI:264, GRP:0, IOB:156, VBUS:0, VPU:0, SMU:117
LCD: Post Parallel Device Initialize..
DLP260X_Done Wait count = 1 msec 
emu_done reset = 0x1, ret = 0x1 
MAX8903A DC input =1 
DPP260X Initialize Success!
DLP : 854X480 RGB888 Mode
DPP260X Current Red=0x32A,Green=0x32A,Blue=0x32A
FWDN: checking...
Loading...
length = 0x67C000, CRC = 0xFC025306
Load Ok! Jump to 0x40300000 (0xEA000016)

Linux version 2.6.28 (phking@linux-hth0) (gcc version 4.3.2 (Sourcery G++ Lite 2008q3-72) ) #3 Mon Jul 12 13:28:17 KST 2010
CPU: ARMv6-compatible processor [410fb766] revision 6 (ARMv7), cr=00c5387f
CPU: VIPT nonaliasing data cache, VIPT nonaliasing instruction cache
Machine: Samsung Oberon Board
Warning: bad configuration page, trying to continue
Memory policy: ECC disabled, Data cache writeback
create_mapping:0x40200000->0xc0000000(0x5000000)
create_mapping:0x402a9000->0xffff0000(0x1000)
create_mapping:0xf0000>0xf0000000(0x100000)
create_mapping:0xf0100000->0xf0100000(0x100000)
create_mapping:0xf0200000->0xf0200000(0x100000)
create_mapping:0xf0300000->0xf0300000(0x100000)
create_mapping:0xf0400000->0xf0400000(0x100000)
create_mapping:0xf0500000->0xf050000000000)
create_mapping:0xf0600000->0xf0600000(0x100000)
create_mapping:0xf0700000->0xf0700000(0x100000)
create_mapping:0x10000000->0xeff00000(0x100000)
Serial Number: 0000000e0dd3c3b0
Built 1 zonelists in Zone order, mobility grouping on.  Total pages20
Kernel command line: console=ttySAC0 lpj=2523136 initcall_debug=0   mem=80M
tcc8900_irq_init
PID hash table entries: 512 (order: 9, 2048 bytes)
 ### CORE CLOCK (506250000 Hz), BUS CLOCK (280000000 Hz) ###
Console: colour dummy device 80x30
console [ttySAC0] enabled
Dentry cache hash table entries: 16384 (ord4, 65536 bytes)
Inode-cache hash table entries: 8192 (order: 3, 32768 bytes)
_etext:0xc0762000, _text:0xc04f5000, _end:0xc07a5db8, __data_start:0xc0762000, __init_end:0xc04f5000, __init_begin:0xc0100000
Memory: 80MB = 80MB total
Memory: 74240KB available (2484K code, 271K data, 4052K init)
Calibrating delay loop (skipped) preset value.. 504.62 BogoMIPS (lpj=2523136)
Mount-cache hash table entries: 512
CPU: Testing write buffer coherency: ok
net_namespace: 288 bytes
NET: Registered protocol family 16
attached TCC adc driver
SCSI subsystem initialized
usbcore: registered new interface driver usbfs
usbcore: registered new interface driver hub
usbcore: registered new device driver usb
NET: Registered protocol family 2
IP route cache hash table entries: 1024 (order: 0, 4096 bytes)
TCP established hash table entries: 4096 (order: 3, 32768 bytes)
TCP bind hash table entries: 4096 (order: 2, 16384 bytes)
TCP: Hash tables configured (established 4096 bind 4096)
TCP reno registered
NET: Registered protocol family 1
Telechips Dynamic Power Management.
NetWinder Floating Point Emulator V0.97 (double precision)
fuse init (API version 7.10)
msgmni has been set to 145
Block layer SCSI generic (bsg) driver version 0.4 loaded (major 253)
io scheduler noop registered
io scheduler cfq registered (default)
i2c /dev entries driver
tcc-i2c tcc-i2c: i2c-0: I2C adapter
cs42l52_i2c_probe: 
fb[0]::map_video_memory: dma=45200000 cpu=c6000000 size=00800000
fb0: tccfb frame buffer device
fb[1]::map_video_memory: dma=45a00000 cpu=c7000000 size=00400000
fb1: tccfb frame buffer device
fb[2]::map_video_memory: dma=45e00000 cpu=c7800000 size=00400000
fb2: tccfb frame buffer device
tcc proc filesystem initialised
OBERON_PWR Driver
oberon_p5v_control: onoff = 1 
Obenron MAX8903A Driver
Battery Voltage = 0.4 
MAX8903A: DC Connected!
Obenron TW8816 Driver
tcc_intr: init (ver 2.1)
tcc_pwm: init (ver 0.1)
Obenron DPP260x Driver
DPP260X Check ID Success!.. 
Obenron TCC_WDT Driver
OBERON_MEM Driver
Obenron OBERON_LEDS Driver
TimerX init(TX-Timer0).
tcc8900-uart.0: tcc-uart0 at MMIO 0xf0532000 (irq = tcc8900-uart.1: tcc-uart1 at MMIO 0xf0532100 (irq = loop: module loaded
TRACE: DPM is now installed
Driver 'sd' needs updating - please use bus_type methods
Initializing USB Mass Storage driver...
usbcore: registered new interface driver usb-storage
USB Mass Storage support registered.
dwc_otg: version 2.60a 22-NOV-2006
DWC_otg: Internal DMA Mode...
DWC_otg: Dedicated Tx FIFOs mode
DWC_otg: Using DMA mode
dwc_otg dwc_otg.0: DWC OTG Controller
dwc_otg dwc_otg.0: new USB bus registered, assigned bus number 1
dwc_otg dwc_otg.0: irq 48, io mem 0x00000000
DWC_otg: Init: Port Power? op_state=1
DWC_otg: Init: Power Port (0)
usb usb1: configuration #1 chosen from 1 choice
hub 1-0:1.0: USB hub found
hub 1-0:1.0: 1 port detected
mice: PS/2 mouse device common for all mice
Oberon GPIO Keypad Driver...
input: Oberon keypad as /class/input/input0
TCC RTC, (c) 2009, Telechips 
tcc-rtc tcc-rtc: rtc core: registered tcc-rtc as rtc0
tcc-sdhc1: init
Set ID to device mode
Advanced Linux Sound Architecture Driver Version 1.0.18rc3.
ASoC version 0.13.2
TCC89XX  cs42l52 ALSA SoC driver ver1.1 10/2008
asoc: CS42L52 <-> tcc-i2s mapping ok
CS42L52: Cirrus CS42L52 codec , revision 3
soc_cs42l52_write: Can't set Limiter 
ID change ISR : Device
DWC_otg: Internal DMA Mode...
soc_cs42l52_setup: <Mono Speaker out>.
DWC_otg: USB RESET
CS42L52: Cirrus Logic ALSA SoC codec cs42l52 driver verison 1.1 10/2008
ALSA device list:
  #0: TCC89XX (CS42L52)
TCP cubic registered
NET: Registered protocol family 17
VFP support v0.3: implementor 41 architecture 1 part 20 variant b rev 5
tcc-rtc tcc-rtc: hctosys: invalid date/time
DWC_otg: Internal DMA Mode...
hp_detect_work:  SPK Sound 
oberon_volume = 0, highlow=1 
init started:  BusyBox v1.4.2 (2010-05-20 22:17:07 KST) multi-call binary
Starting pid 352, console /dev/console: '/etc/init.d/rcS'
run /etc/init.d/rcS 
mdev init: ...
TCC8900_nand: module license 'Proprietary. (C) 2008 Telechips Inc.' taints kernel.
Dumy-Ready Interrupt-!!!!!!!!!!!!!!!!
[NAND        ] [PORT CONFIG - CS[0, 1] [NAND Data Port: GPIO_B Hw4 ~ Hw11]
[NAND        ] [BClk 156MHZ][1Tick 65][RE-S:0,P:7,H:4][WR-S:0,P:6,H:4][COM-S:2,P:15,H:15]
[NAND        ] [NB Area:4MB][DT Area:931MB][HD Area0:9MB][HD Area1:9MB]
 ndda: ndda1 ndda2 ndda3
[tcc_nand] init ndd(TCC8900, V7018)
[tcc-gadget]fsg_init start
[tcc-gadget]fsg_bind start
fsg_bind: gadget->name = dwc_otg_pcd 
usb_gadget_controller_number: gadget->name = dwc_otg_pcd 
g_file_storage gadget: File-backed Storage Gadget, version: 7 August 2007
g_file_storage gadget: Number of LUNs=1
[tcc-gadget]fsg init end
pwd = /
OBERON Update Script(pre)....
[Update File Check] Update Files Not Exist...
dosfsck_check pwd=/
[/usr/sbin/dosfsck] /dev/ndda1 .. ok
dosfsck_check pwd=/
[/usr/sbin/dosfsck] /dev/ndda2 .. ok
dosfsck_check pwd=/
[/usr/sbin/dosfsck] /dev/ndda3 .. ok
Starting pid 427, console /dev/ttySAC0: '/bin/sh'
/ #
```

## Shell

```
/ # ls -al
drwxr-xr-x   18 root     root            0 Jan  1 00:00 .
drwxr-xr-x   18 root     root            0 Jan  1 00:00 ..
drwxr-xr-x    2 1013     100             0 Jul 12  2010 bin
drwxr-xr-x    3 root     root        16384 Jan  1 00:00 data
drwxr-xr-x    6 1013     100             0 Jan  1 00:00 dev
drwxr-xr-x    6 root     root        16384 Jan  1 00:00 disk
drwxr-xr-x    5 1013     100             0 Jul 12  2010 etc
lrwxrwxrwx    1 1013     100            13 Jul 12  2010 init -> ./bin/busybox
drwxr-xr-x    3 1013     100             0 Jul 12  2010 lib
lrwxrwxrwx    1 1013     100            11 Jul 12  2010 linuxrc -> bin/busybox
drwxr-xr-x    3 1013     100             0 Jul 12  2010 mnt
drwxr-xr-x    2 1013     100             0 Jul 12  2010 nfs
lrwxrwxrwx    1 root     root           11 Jan  1 00:00 opt -> /system/opt
dr-xr-xr-x   34 root     root            0 Jan  1 00:00 proc
drwxr-xr-x    2 1013     100             0 Jul 12  2010 sbin
drwxr-xr-x   13 root     root            0 Jan  1 00:00 sys
drwxr-xr-x    2 root     root        16384 Jan  1 00:00 system
drwxr-xr-x    2 1013     100             0 Jul 12  2010 temp
drwxr-xr-x    2 1013     100             0 Jul 12  2010 tmp
drwxr-xr-x    6 1013     100             0 Jul 12  2010 usr
drwxr-xr-x    6 root     root            0 Jan  1 00:00 var
/ # cat /proc/cpuinfo 
Processor	: ARMv6-compatible processor rev 6 (v6l)
BogoMIPS	: 504.62
Features	: swp half thumb fastmult vfp edsp java 
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x0
CPU part	: 0xb76
CPU revision	: 6

Hardware	: Samsung Oberon Board
Revision	: AX
Oberon Version	: 0007.2
Serial		: 0000000e0dd3c3b0
/ # cat /proc/partitions 
major minor  #blocks  name

 240        0     953344 ndda
 240        1     184320 ndda1
 240        2     748528 ndda2
 240        3      20480 ndda3
/ # mount
rootfs on / type rootfs (rw)
proc on /proc type proc (rw)
none on /dev/pts type devpts (rw,mode=622)
tmpfs on /dev/shm type tmpfs (rw)
sysfs on /sys type sysfs (rw)
/dev/ndda1 on /system type vfat (ro,noatime,fmask=0022,dmask=0022,codepage=cp437,iocharset=iso8859-1,shortname=mixed,utf8,errors=ro)
/dev/ndda2 on /disk type vfat (rw,noatime,fmask=0022,dmask=0022,codepage=cp437,iocharset=iso8859-1,shortname=mixed,utf8,errors=ro)
/dev/ndda3 on /data type vfat (rw,noatime,fmask=0022,dmask=0022,codepage=cp437,iocharset=iso8859-1,shortname=mixed,utf8,errors=ro)
tmpfs on /dev/shm type tmpfs (rw)
/ # cat /etc/passwd 
root::0:0:root:/root:/bin/sh
/ # find . -name bin -type d
./bin
./usr/bin
/ # ls bin/
ash      date     false    kill     mount    pwd      su       uname
busybox  dd       fgrep    ln       msh      rm       sync     usleep
cat      df       grep     login    mv       rmdir    tar      vi
chmod    dmesg    gunzip   ls       nice     sed      touch
chown    echo     gzip     mkdir    ping     sh       true
cp       egrep    ip       mknod    ps       sleep    umount
/ # ls /usr/bin/
[              du             ftpput         passwd         udevd
[[             eject          hexdump        telnet         uptime
amixer         env            killall        test           which
aplay          expr           md5sum         time           yes
awk            find           mesg           top
blkid          free           ntfs-3g        tr
clear          ftpget         ntfs-3g.probe  udevadm
```
