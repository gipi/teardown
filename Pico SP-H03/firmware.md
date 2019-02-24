# Firmware

Seems from the firmware (downloaded from the site) named ``P-OBRNPWWC-1008.1.rom``
that is a ``tar`` archive, that the system is a linux one.

You can find the complete list of files included in this link

 - [P-OBRNPWWC-1008.1.rom list files](P-OBRNPWWC-1008.1.list) generate with ``find Pico\ SP-H03/P-OBRNPWWC-1008.1/ -exec file \{\} \; | cut -d'/' -f3- > Pico\ SP-H03/P-OBRNPWWC-1008.1.list``

below just a snippet to show a glipse of what you can find

```
$ tar -tf Pico\ SP-H03/P-OBRNPWWC-1008.1.rom
./
./wplayer
./opt/
./opt/qt/
./opt/qt/lib/
./opt/qt/lib/libQtNetwork.so.4
./opt/qt/lib/libQtGui.so.4
./opt/qt/lib/fonts/
./opt/qt/lib/fonts/unifont_160_50.qpf
./opt/qt/lib/fonts/Oberon_chn_kor_20091217.ttf
./opt/qt/lib/fonts/langpack_cjk.dat
./opt/qt/lib/libQtCore.so.4
./pico_player
./ui/
  more stuffs
./TCCKernel7.3.rom
./setup/
./setup/reg_set.obn
./md5sum
./TCCBoot4.2.rom
./run.sh
./pico
```

Since it is an Android system, we can assume that this archive is what contained in the ``userdata``
partition (less the kernel and bootloader probably).

Analysing the ``TCCKernel7.3.rom`` and ``TCCBoot4.2.rom`` that I assume are respectively the kernel
and bootloader

```
$binwalk Pico\ SP-H03/P-OBRNPWWC-1008.1/TCCBoot4.2.rom 

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------

$ binwalk Pico\ SP-H03/P-OBRNPWWC-1008.1/TCCKernel7.3.rom 

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
55177         0xD789          Certificate in DER format (x509 v3), header length: 4, sequence length: 1416
59013         0xE685          Certificate in DER format (x509 v3), header length: 4, sequence length: 13588
78208         0x13180         gzip compressed data, maximum compression, from Unix, last modified: 2010-07-27 01:42:34
4720605       0x4807DD        Certificate in DER format (x509 v3), header length: 4, sequence length: 5376
5024709       0x4CABC5        Certificate in DER format (x509 v3), header length: 4, sequence length: 1328
5419085       0x52B04D        Certificate in DER format (x509 v3), header length: 4, sequence length: 4866
5531093       0x5465D5        Certificate in DER format (x509 v3), header length: 4, sequence length: 1292
5702529       0x570381        Certificate in DER format (x509 v3), header length: 4, sequence length: 13588
6446763       0x625EAB        Linux kernel version 2.6.28
6477872       0x62D830        CRC32 polynomial table, little endian
6821729       0x681761        Unix path: /dev/vc/0
6834593       0x6849A1        IMG0 (VxWorks) header, size: 1111564346
```

looking at the strings inside you can have some clues

```
$ strings Pico\ SP-H03/P-OBRNPWWC-1008.1/TCCBoot4.2.rom | grep -i tele
TELECHIPS01
TELECHIPS02
TELECHIPS03
TELECHIPS04
$ strings Pico\ SP-H03/P-OBRNPWWC-1008.1/TCCBoot4.2.rom | grep -i boot
    tcboot ver %s for %s Linux
tcboot.rom
Updating tcboot.rom -> 
$ strings Pico\ SP-H03/P-OBRNPWWC-1008.1/TCCKernel7.3.rom | grep -i samsung
Samsung Oberon Board
Samsung Info. Systems America, Inc.
Samsung
SAMSUNG
$ strings Pico\ SP-H03/P-OBRNPWWC-1008.1/TCCBoot4.2.rom | grep -i usb
SIGBYAHONG_USB_MANAGER_LINUX_TCC89XX_V2.000
SIGBYAHONG_USBPHY_LINUX_TCC89XX_V2.000
SIGBYAHONG_USB_DEVICE_LINUX_TCC89XX_V2.000
MAX8903A: USB Input.
```

Lurking on github I find [code](https://github.com/JeffreyLau/JJWD-K8_icsCream/blob/a9790f6edf973d9e6b102f9be89c7b7f883f1cb2/bootable/bootloader/lk/platform/tcc_shared/update.c) that
looks like the bootloader code.

