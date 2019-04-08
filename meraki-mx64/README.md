# Meraki MX64

## Serial

Connector J1

| Pin | Signal |
|-----|--------|
| 1   | VCC |
| 2   |  TX |
| 3   |  RX |
| 4   | GND |

## Recovery

If you keep pressing the reset button during the startup, after a while it reboots
in a initramfs shell (release the reset button and keep pressing enter).

```
[   67.570000] Mapping 9596928 bytes for /dev/mtdblock/diagnostic1
[   69.170000] UBIFS: un-mount UBI device 0, volume 3
[   69.180000] Starting new kernel
[   69.180000] Bye!
Uncompressing Linux...00000800 000b4df8  done, booting the kernel.
console [ttyS0] enabled, bootconsole disabled
serial8250.0: ttyS1 at MMIO 0x18000400 (irq = 117) is a 16550
brd: module loaded
loop: module loaded
bcmsflash: found no supported devices
early_nvram_get: Failed reading nvram var bootpartition
early_nvram_get: Failed reading nvram var image_first_offset
early_nvram_get: Failed reading nvram var image_second_offset
Boot partition size = 1048576(0x100000)
lookup_nflash_rootfs_offset: offset = 0x1000000
nflash: ubifs with lzo found at block #64 offset 1000000
Creating 5 MTD partitions on "nflash":
0x000000000000-0x000000100000 : "boot"
0x000000400000-0x000000500000 : "nvram"
0x000001000000-0x000002000000 : "linux"
0x000001000000-0x000002000000 : "rootfs"
0x000000000000-0x000006000000 : "senao"
PPP generic driver version 2.4.2
NET: Registered protocol family 24
u32 classifier
    Actions configured
Netfilter messages via NETLINK v0.30.
nf_conntrack version 0.5.0 (16053 buckets, 64212 max)
ctnetlink v0.93: registering with nfnetlink.
xt_time: kernel timezone is -0000
ip_tables: (C) 2000-2006 Netfilter Core Team
TCP cubic registered
NET: Registered protocol family 10
ip6_tables: (C) 2000-2006 Netfilter Core Team
IPv6 over IPv4 tunneling driver
NET: Registered protocol family 17
802.1Q VLAN Support v1.8 Ben Greear <greearb@candelatech.com>
All bugs added by David S. Miller <davem@redhat.com>
Warning: unable to open an initial console.
Freeing init memory: 7568K
/sbin/hotplug2: No such file or directory
insmod: ipv6.ko: no module by that name found
insmod: bcm57xx.ko: no module by that name found
hotplug detected product:  1d6b/3/206
hotplug detected product:  1d6b/3/206
hotplug detected product:  1d6b/2/206
hotplug detected product:  1d6b/2/206
hotplug detected product:  1d6b/1/206
hotplug detected product:  1d6b/1/206
Hit enter to continue...vlan1: WLC_GET_VAR(cap): Operation not supported
eth1: No such device
ifconfig: No such device
eth2: No such device
ifconfig: No such device
ip: cannot find device "eth1"
info, udhcp server (v0.9.8) started
info, udhcp server (v0.9.8) started
telnetd: starting
  port: 23; interface: br0; login program: /bin/sh
telnetd: starting
  port: 23; interface: br1; login program: /bin/sh
killall: hspotap: no process killed
BSD is not enabled: 0=0
ACSD >>main(570): No interface specified, exiting...Hit enter to continue...connect: Network is unreachable
Hit enter to continue...


BusyBox v1.7.2 (2015-03-26 20:02:08 PDT) built-in shell (ash)
Enter 'help' for a list of built-in commands.

#
# uname -a
Linux (none) 2.6.36.4brcmarm #219 SMP PREEMPT Thu Mar 26 20:03:16 PDT 2015 armv7l unknown
# cat /proc/cpu
/proc/cpu/     /proc/cpuinfo
# cat /proc/cpuinfo 
Processor       : ARMv7 Processor rev 0 (v7l)
processor       : 0
BogoMIPS        : 2398.61

processor       : 1
BogoMIPS        : 2398.61

Features        : swp half thumb fastmult edsp 
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x3
CPU part        : 0xc09
CPU revision    : 0

Hardware        : Northstar Prototype
Revision        : 0000
Serial          : 0000000000000000
# find . -name \*.ko
./lib/modules/2.6.36.4brcmarm/kernel/crypto/tcrypt.ko
./lib/modules/2.6.36.4brcmarm/kernel/crypto/ocf/cryptosoft.ko
./lib/modules/2.6.36.4brcmarm/kernel/crypto/ocf/ocf-bench.ko
./lib/modules/2.6.36.4brcmarm/kernel/crypto/ocf/cryptodev.ko
./lib/modules/2.6.36.4brcmarm/kernel/crypto/ocf/ocf.ko
./lib/modules/2.6.36.4brcmarm/kernel/crypto/ocf/ocfnull/ocfnull.ko
./lib/modules/2.6.36.4brcmarm/kernel/lib/libcrc32c.ko
./lib/modules/2.6.36.4brcmarm/kernel/net/sched/sch_hfsc.ko
./lib/modules/2.6.36.4brcmarm/kernel/net/sched/sch_tbf.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/scsi/scsi_mod.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/scsi/scsi_wait_scan.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/scsi/sd_mod.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/net/dpsta/dpsta.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/net/igs/igs.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/net/ctf/ctf.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/net/wl/wl.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/net/et/et.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/net/emf/emf.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/connector/cn.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/usb/storage/usb-storage.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/usb/host/ehci-hcd.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/usb/host/xhci-hcd.ko
./lib/modules/2.6.36.4brcmarm/kernel/drivers/usb/host/ohci-hcd.ko
./src/bcmdrivers/crypto/iproc_crypto.ko
./src/bcmdrivers/fa2/iproc_fa2.ko
./src/bcmdrivers/pae/paemsg/iproc_pae.ko
# ps
  PID  Uid        VSZ Stat Command
    1 0          1228 S   /sbin/preinit
    2 0               SW  [kthreadd]
    3 0               SW  [ksoftirqd/0]
    4 0               SW  [kworker/0:0]
    5 0               SW  [kworker/u:0]
    6 0               SW  [migration/0]
    7 0               SW  [migration/1]
    8 0               SW  [kworker/1:0]
    9 0               SW  [ksoftirqd/1]
   10 0               SW< [khelper]
   11 0               SW  [kworker/u:1]
   61 0               SW  [sync_supers]
   63 0               SW  [bdi-default]
   64 0               SW< [kblockd]
   70 0               SW  [khubd]
  119 0               SW  [kswapd0]
  166 0               SW  [fsnotify_mark]
  176 0               SW< [crypto]
  251 0               SW  [kworker/1:1]
  252 0               SW  [kworker/0:1]
  255 0               SW  [mtdblock0]
  260 0               SW  [mtdblock1]
  265 0               SW  [mtdblock2]
  270 0               SW  [mtdblock3]
  275 0               SW  [mtdblock4]
  418 0          1312 S   httpd /tmp/httpd.conf
  423 0           692 S   udhcpd /tmp/udhcpd0.conf
  424 0           760 S   /usr/sbin/dnsmasq -h -n -i br0 -i br1 -r /tmp/resolv.
  426 0           692 S   udhcpd /tmp/udhcpd1.conf
  428 0          1160 S   igd -D
  547 0          1284 S   nas
  580 0           648 S   utelnetd -d -i br0
  586 0           648 S   utelnetd -d -i br1
  589 0           732 S   igmp eth1
  594 0           964 S   lld2d br0
  608 0          1112 S   /bin/sh
 2416 0          1100 R   ps
# ifconfig
br0       Link encap:Ethernet  HWaddr 00:22:44:66:88:00
          inet addr:192.168.1.1  Bcast:192.168.1.255  Mask:255.255.255.0
          inet6 addr: fe80::222:44ff:fe66:8800/64 Scope:Link
          UP BROADCAST RUNNING ALLMULTI MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:60 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:0
          RX bytes:0 (0.0 B)  TX bytes:15976 (15.6 KiB)

br1       Link encap:Ethernet  HWaddr 16:99:3B:3F:F1:03
          inet addr:192.168.2.1  Bcast:192.168.2.255  Mask:255.255.255.0
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:43 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:0
          RX bytes:0 (0.0 B)  TX bytes:15022 (14.6 KiB)

eth0      Link encap:Ethernet  HWaddr 00:22:44:66:88:00
          inet6 addr: fe80::222:44ff:fe66:8800/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:202 errors:0 dropped:0 overruns:0 frame:0
          TX packets:435 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:16350 (15.9 KiB)  TX bytes:183292 (178.9 KiB)
          Interrupt:179 Base address:0x2000

lo        Link encap:Local Loopback
          inet addr:127.0.0.1  Mask:255.0.0.0
          inet6 addr: ::1/128 Scope:Host
          UP LOOPBACK RUNNING MULTICAST  MTU:16436  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:0
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

vlan1     Link encap:Ethernet  HWaddr 00:22:44:66:88:00
          inet6 addr: fe80::222:44ff:fe66:8800/64 Scope:Link
          UP BROADCAST RUNNING ALLMULTI MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:329 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:0
          RX bytes:0 (0.0 B)  TX bytes:31424 (30.6 KiB)
```

## Mounting external USB pen

```
# insmod scsi_mod
# insmod usb-storage
```

doesn't work

## Links

 - [Wiki page](https://wikidevi.com/wiki/Cisco_Meraki_MX64) with some specifications
 - [Github repo](https://github.com/riptidewave93/meraki-uboot) Copies of the Cisco Meraki U-Boot GPL Source. Compile with ``gcc-4``, branch ``mr32-20171031`` and target ``venom_nand``.
 ```
 $ make CROSS_COMPILE=arm-none-eabi- venom_nand_config
 $ make CROSS_COMPILE=arm-none-eabi- venom_nand
 ```
 
 - [Pwning the meraki MR18](https://snt.sh/2016/02/pwning-the-meraki-mr18/)
 - [Pwning the meraki MR18 again](https://snt.sh/2017/06/pwning-the-meraki-mr18-again/)
