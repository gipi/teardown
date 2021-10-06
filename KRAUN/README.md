# KRAUN KR.YL

[This](https://www.manualslib.com/manual/788619/Kraun-Wireless-Modem-Router-Adsl2-2Plus.html) seems to be the user manual.

Connect the ethernet cable

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
