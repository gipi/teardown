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

## Links

 - [DSL-302T demystified](http://www.webalice.it/andrea.usenet/dsl-302t.htm)
 - [MIPS](https://www.linux-mips.org/wiki/DSL-xxxT)

