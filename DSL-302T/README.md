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

## JTAG

Connector ``JP1``
```
     14 1 GND
     13 2 GND
     12 3 GND
     11 4 GND
     10 5 GND
      9 6 
      8 7
```

## Links

 - [DSL-302T demystified](http://www.webalice.it/andrea.usenet/dsl-302t.htm)
 - [MIPS](https://www.linux-mips.org/wiki/DSL-xxxT)

