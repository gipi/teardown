# Remarkable 2

|             |                                                       |
|-------------|-------------------------------------------------------|
| Size        | 187 x 246 x 4.7mm                                     |
| Weight      | Approximately 403.5 gram (0.88 lb)                    |
| Display     | 10.3” monochrome                                      |
| Resolution  | 1872x1404 resolution (226 DPI)                        |
| Digitizer   | EMR (electromagnetic resonance) digitizer from Wacom  |
| Storage     | 8GB                                                   |
| RAM         | 1024 MB LPDDR3 SDRAM                                  |
| Battery     | 3000 mAh                                              |
| CPU         | 1.2 GHz dualCore ARM CPU                              |
| OS          | Codex, a custom Linux(4.14.78)-based OS               |

It's possible to connect via ``ssh`` (the password is indicated in the ``Settings > Help > Copyrights and licenses > General information > GPLv3 compliance``)

```
$ ssh -oHostKeyAlgorithms=+ssh-rsa root@192.168.1.100
root@192.168.1.100's password: 
ｒｅＭａｒｋａｂｌｅ
╺━┓┏━╸┏━┓┏━┓   ┏━┓╻ ╻┏━╸┏━┓┏━┓
┏━┛┣╸ ┣┳┛┃ ┃   ┗━┓┃ ┃┃╺┓┣━┫┣┳┛
┗━╸┗━╸╹┗╸┗━┛   ┗━┛┗━┛┗━┛╹ ╹╹┗╸
reMarkable: ~/
```

```
reMarkable: ~/ uname -a
Linux reMarkable 5.4.70-v1.1.5-rm11x #1 SMP PREEMPT Fri Nov 12 14:59:18 UTC 2021 armv7l GNU/Linux
```

```
reMarkable: ~/ ls /boot/ -al
drwxr-xr-x    2 root     root          1024 Feb  2 13:38 .
drwxr-xr-x   18 root     root          1024 Feb  2 13:41 ..
-rw-r--r--    1 root     root        651308 Nov 12 15:34 u-boot-zero-sugar_config-v2018.07+gitAUTOINC+47c91918aa-r0.imx
-rw-r--r--    1 root     root        651308 Nov 12 15:34 u-boot.imx
lrwxrwxrwx    1 root     root            62 Nov 15 09:22 u-boot.imx-zero-sugar_config -> u-boot-zero-sugar_config-v2018.07+gitAUTOINC+47c91918aa-r0.imx
lrwxrwxrwx    1 root     root            26 Nov 15 09:22 zImage -> zImage-5.4.70-v1.1.5-rm11x
-rw-r--r--    1 root     root       5322288 Nov 15 09:22 zImage-5.4.70-v1.1.5-rm11x
-rw-r--r--    1 root     root         45868 Nov 15 09:22 zero-sugar.dtb
```

```
reMarkable: ~/ cat /proc/cpuinfo 
processor       : 0
model name      : ARMv7 Processor rev 5 (v7l)
BogoMIPS        : 16.00
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm 
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xc07
CPU revision    : 5

processor       : 1
model name      : ARMv7 Processor rev 5 (v7l)
BogoMIPS        : 16.00
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm 
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xc07
CPU revision    : 5

Hardware        : Freescale i.MX7 Dual (Device Tree)
Revision        : 0000
Serial          : xxxxxxxxxx
```

https://remarkablewiki.com/tech/rm2_otg_pogo

```
reMarkable: ~/ ps
  PID USER       VSZ STAT COMMAND
    1 root     19804 S    {systemd} /sbin/init
    2 root         0 SW   [kthreadd]
    3 root         0 IW<  [rcu_gp]
    4 root         0 IW<  [rcu_par_gp]
    8 root         0 IW<  [mm_percpu_wq]
    9 root         0 SW   [ksoftirqd/0]
   10 root         0 IW   [rcu_preempt]
   11 root         0 SW   [migration/0]
   12 root         0 SW   [cpuhp/0]
   13 root         0 SW   [cpuhp/1]
   14 root         0 SW   [migration/1]
   15 root         0 SW   [ksoftirqd/1]
   18 root         0 SW   [kdevtmpfs]
   19 root         0 SW   [rcu_tasks_kthre]
   22 root         0 SW   [oom_reaper]
   23 root         0 IW<  [writeback]
   24 root         0 SW   [kcompactd0]
   68 root         0 IW<  [kblockd]
   69 root         0 SW   [irq/236-bd7181x]
   70 root         0 SW   [watchdogd]
   71 root         0 IW<  [rpciod]
   72 root         0 IW<  [kworker/u5:0]
   73 root         0 IW<  [xprtiod]
   74 root         0 SW   [kswapd0]
   75 root         0 IW<  [nfsiod]
   77 root         0 SW   [irq/67-max77818]
   78 root         0 SW   [irq/67-max77818]
   79 root         0 SW   [irq/67-max77818]
   80 root         0 IW<  [ci_otg]
   81 root         0 IW<  [ci_power_lost]
   82 root         0 SW   [irq/61-wacom_i2]
   83 root         0 SW   [irq/74-2-0024]
   84 root         0 IW<  [sdhci]
   85 root         0 SW   [irq/53-mmc1]
   87 root         0 IW<  [sdhci]
   88 root         0 SW   [irq/54-mmc2]
   90 root         0 IW<  [ipv6_addrconf]
   92 root         0 IW<  [mmc_complete]
   95 root         0 SW   [irq/23-imx_ther]
   96 root         0 SW   [irq/100-one_wir]
   97 root         0 IW<  [ci_otg]
   98 root         0 IW<  [ci_power_lost]
  100 root         0 SW   [jbd2/mmcblk2p2-]
  101 root         0 IW<  [ext4-rsv-conver]
  132 root      7596 S    /usr/sbin/haveged -w 1024 -v 1 --Foreground
  133 root     29028 S    /lib/systemd/systemd-journald
  134 root     12196 S    /lib/systemd/systemd-udevd
  136 systemd- 10688 S    /lib/systemd/systemd-networkd
  152 systemd- 19552 S    /lib/systemd/systemd-timesyncd
  163 root         0 IW<  [cfg80211]
  171 root      2740 S    /usr/sbin/ifplugd -n -i usb0
  172 messageb  4084 S    /usr/bin/dbus-daemon --system --address=systemd: --nofork --nopidfile --systemd-activation --syslog-only
  174 root      3924 S    /sbin/agetty -o -p -- \u --noclear tty1 linux
  175 root      1876 S    /sbin/agetty -8 -L ttymxc0 115200 xterm
  176 root     66852 S    /usr/bin/sync --service
  180 root     31244 S    /usr/sbin/update_engine -foreground
  207 root     10004 S    /lib/systemd/systemd-logind
  215 root      2672 S    /usr/sbin/dhcpcd -4 -b --master wlan0
  220 root         0 SW   [jbd2/mmcblk2p4-]
  221 root         0 IW<  [ext4-rsv-conver]
  228 root     12068 S    /usr/bin/crashuploader --path /home/root/
  230 root      432m S    /usr/bin/xochitl --system
  248 root      7548 S    /usr/sbin/wpa_supplicant -u
  289 root     12032 S    /usr/bin/qmdns-responder
 1355 root         0 IW<  [kworker/0:2H-mm]
 1443 root         0 IW   [kworker/u4:2-ev]
 1575 root         0 IW<  [kworker/1:1H-kb]
 1590 root         0 IW   [kworker/1:2-eve]
 1652 root         0 IW   [kworker/1:0-eve]
 1657 root         0 DW   [kworker/0:1-eve]
 1670 root         0 IW<  [kworker/1:2H-kb]
 1673 root         0 IW   [kworker/u4:1-ev]
 1709 root         0 DW   [kworker/0:3+eve]
 1710 root         0 IW<  [brcmf_wq/mmc1:0]
 1711 root         0 SW   [brcmf_wdog/mmc1]
 1762 root         0 IW<  [kworker/0:0H]
 1763 root      2592 R    /usr/sbin/dropbear -i -r /etc/dropbear/dropbear_rsa_host_key -B
 1764 root      3180 S    -sh
 1772 root         0 IW   [kworker/1:1-eve]
 1773 root         0 IW   [kworker/u4:0-br]
 1775 root         0 IW   [kworker/u4:3-ev]
 1779 root      2832 R    ps
```

```
reMarkable: ~/ cat /proc/partitions 
major minor  #blocks  name

 179        0    7471104 mmcblk2
 179        1      20480 mmcblk2p1
 179        2     276480 mmcblk2p2
 179        3     276480 mmcblk2p3
 179        4    6896640 mmcblk2p4
```

[xochitl](https://remarkablewiki.com/tech/xochitl) is the main process running the GUI of the reMarkable. It is currently closed source.

## Hacking

It's a freescale based system so you can use the standard tool
released by the company, in particular [ddvk/remarkable2-recovery](https://github.com/ddvk/remarkable2-recovery)
has a step-by-step instructions.


[reMarkable/linux](https://github.com/reMarkable/linux) is the source code for the Linux kernel
for reMarkable 1 & 2. zero-sugar is rM2 and zero-gravitas is rM1. zero-colors is Linux 5.4 for both rM1 and rM2.

