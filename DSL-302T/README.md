# DSL-302T

Known in Italy as **Alice ADSL 302T**.

```
$ nmap 192.168.1.1

Starting Nmap 7.01 ( https://nmap.org ) at 2017-05-26 09:46 CEST
Nmap scan report for 192.168.1.1
Host is up (0.035s latency).
Not shown: 998 closed ports
PORT   STATE SERVICE
23/tcp open  telnet
80/tcp open  http

Nmap done: 1 IP address (1 host up) scanned in 13.57 seconds
```

```
$ telnet 192.168.1.1
Trying 192.168.1.1...
Connected to 192.168.1.1.
Escape character is '^]'.

login: root
Password: admin


BusyBox v0.61.pre (2004.03.11-17:08+0000) Built-in shell (ash)
Enter 'help' for a list of built-in commands.

#
```

## Processor

[TNETD7300GDU AR7](https://www.linux-mips.org/wiki/AR7) chip.

## UART

It's the connector ``JP2``

```
1 RX
2 GND
3 3v3
4 GND
5 TX
```

The baud rate is 38400. This allows to access the bootloader (``ADAM2``)

```
Warning!!
Default IP address [10.8.8.8] maybe conflict with other device(s)!!
Please Change IP by: setenv my_ipaddress <your ip address>

ADAM2 Revision 0.22.02
Usage: setmfreq [-d] [-s sys_freq, in MHz] [cpu_freq, in MHz]
Memory optimization Complete!

Adam2_AR7DB > 
Press any key to abort OS load, or wait 5 seconds for OS to boot...

Adam2_AR7DB > help
         Commands               Description
         --------               -----------
         h/help Displays the commands supported
           info Displays board information
          memop Memory Optimization
       setmfreq configures/dumps the system and cpu frequencies
             dm Dump memory at <address>
          erase Erase Flash except Adam2 Kernel and Env space
       printenv Displays Env. Variables
         setenv Sets Env. variable <var> with a value <val>
       unsetenv Unsets the Env. variable <var>
         fixenv Defragment for Env. space
             go Loads the image starting at address <mtd1>
Adam2_AR7DB > info
Monitor Revision              0.22.02
Monitor Compilation time      Oct 29 2003, 15:47:14
Endianness                    Little
External Memory rate          Full, 16 bit wide
CPU Frequency                 150 MHz
```

## JTAG

Connector ``JP1``
```
nTRST 1 14 GND
TDI   2 13 GND
TDO   3 12 GND
TMS   4 11 GND
TCK   5 10 GND
nSRST 6  9 ?
DINT  7  8 VCC
```

remember that is a ``EJTAG`` so you need a pull-up resistor between ``nTRST`` and ``VCC``.

If I try to dump the flash

```
Adam2_AR7DB > dm 0x90000000

90000000: 40809000 40809800 401a6000 241bfffe - ...@...@.`.@...$
90000010: 035bd024 3c1b0040 035bd025 409a6000 - $.[.@..<%.[..`.@
90000020: 3c1a0080 409a6800 3c029000 24420720 - ...<.h.@...< .B$
90000030: 3c011fff 3421ffff 00411024 3c01a000 - ...<..!4$.A....<
90000040: 00411025 0040f809 00000000 14400021 - %.A...@.....!.@.
90000050: 00000000 3c029000 24420738 3c011fff - .......<8.B$...<
90000060: 3421ffff 00411024 3c01a000 00411025 - ..!4$.A....<%.A.
90000070: 0040f809 00000000 14400016 00000000 - ..@.......@....
```

I can dump it with ``dump_image.py`` and remount the pieces for the bootloader

```
$ ls DSL-302T/cfi.{0..15}.img -v1 | xargs cat > DSL-302T/ADAM2.img
$ radare2 -a mips -m 0x90000000 DSL-302T/ADAM2.img
[0x90000000]> pd 20
/ (fcn) fcn.90000000 172
|   fcn.90000000 ();
|           0x90000000      00908040       mtc0 zero, s2, 0
|           0x90000004      00988040       mtc0 zero, s3, 0
|           0x90000008      00601a40       mfc0 k0, t4, 0
|           0x9000000c      feff1b24       addiu k1, zero, -2
|           0x90000010      24d05b03       and k0, k0, k1
|           0x90000014      40001b3c       lui k1, 0x40
|           0x90000018      25d05b03       or k0, k0, k1
|           0x9000001c      00609a40       mtc0 k0, t4, 0
|           0x90000020      80001a3c       lui k0, 0x80
|           0x90000024      00689a40       mtc0 k0, t5, 0
|           0x90000028      0090023c       lui v0, 0x9000
|           0x9000002c      20074224       addiu v0, v0, 0x720
|           0x90000030      ff1f013c       lui at, 0x1fff
|           0x90000034      ffff2134       ori at, at, 0xffff
|           0x90000038      24104100       and v0, v0, at
|           0x9000003c      00a0013c       lui at, 0xa000
|           0x90000040      25104100       or v0, v0, at
|           0x90000044      09f84000       jalr v0
|           0x90000048      00000000       nop
|       ,=< 0x9000004c      21004014       bnez v0, fcn.900000d4
```

## Links

 - [DSL-302T demystified](http://www.webalice.it/andrea.usenet/dsl-302t.htm)
 - [MIPS](https://www.linux-mips.org/wiki/DSL-xxxT)

