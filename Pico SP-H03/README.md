This is the pico projector for Samsung

Product page: http://www.samsung.com/it/support/model/SP0351VBX/EN

This is the ``/var/log/messages`` when attacched

```
Feb 10 15:07:37 pc kernel: [22312.063237] usb 3-9: new high-speed USB device number 4 using xhci_hcd
Feb 10 15:07:53 pc kernel: [22327.372992] usb 3-9: new high-speed USB device number 5 using xhci_hcd
Feb 10 15:07:53 pc kernel: [22327.502208] usb 3-9: New USB device found, idVendor=140e, idProduct=b058
Feb 10 15:07:53 pc kernel: [22327.502211] usb 3-9: New USB device strings: Mfr=1, Product=2, SerialNumber=3
Feb 10 15:07:53 pc kernel: [22327.502212] usb 3-9: Product: File-backed Storage Gadget
Feb 10 15:07:53 pc kernel: [22327.502213] usb 3-9: Manufacturer: TELECHIPS
Feb 10 15:07:53 pc kernel: [22327.502214] usb 3-9: SerialNumber: 0123456789ABCDEF0123456789ABCDEF
Feb 10 15:07:53 pc mtp-probe: checking bus 3, device 5: "/sys/devices/pci0000:00/0000:00:14.0/usb3/3-9"
Feb 10 15:07:53 pc mtp-probe: bus: 3, device: 5 was not an MTP device
Feb 10 15:07:53 pc kernel: [22327.558831] usb-storage 3-9:1.0: USB Mass Storage device detected
Feb 10 15:07:53 pc kernel: [22327.558888] scsi6 : usb-storage 3-9:1.0
Feb 10 15:07:53 pc kernel: [22327.558941] usbcore: registered new interface driver usb-storage
Feb 10 15:07:54 pc kernel: [22328.559490] scsi 6:0:0:0: Direct-Access     TELECHIP MASS STORAGE     03ff PQ: 0 ANSI: 0 CCS
Feb 10 15:07:54 pc kernel: [22328.559739] sd 6:0:0:0: Attached scsi generic sg2 type 0
Feb 10 15:07:54 pc kernel: [22328.560585] sd 6:0:0:0: [sdc] Attached SCSI removable disk
```

## Firmware

Seems from the firmware (downloaded from the site) named ``P-OBRNPWWC-1008.1.rom``
that is a ``tar`` archive, that the system is a linux one

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
./opensource/
./opensource/opensource_oberon_dutch.txt
./opensource/opensource_oberon_sweden.txt
./opensource/opensource_oberon_spain.txt
./opensource/opensource_oberon_china.txt
./opensource/opensource_oberon_germany.txt
./opensource/opensource_oberon_turky.txt
./opensource/opensource_oberon_russia.txt
./opensource/opensource_oberon_tai.txt
./opensource/opensource_oberon_portugese.txt
./opensource/opensource_oberon_french.txt
./opensource/opensource_oberon_english.txt
./opensource/opensource_oberon_korea.txt
./opensource/opensource_oberon_italia.txt
./md5sum
./TCCBoot4.2.rom
./run.sh
./pico
```

## SOC

Seems the ``TCC9101G-0BX`` an ``ARM`` processor.

## Connectors

Code | Description | Component
-----|-------------|----------
CN201| Connects to the keypad. A bad connection will disable the keypad. |
CN701| Connects to the AV board. A bad connection will disable the PC, AUDIO and VIDEO inputs. |
CN801| Downloads the sub-MICOM and is for technical service purpose only.|
CN901| Connects to the speaker. A bad connection will disable the audio.|
CN1002| Connects to a fan. A bad connection will disable the fan, causing a fan error. |
CN1003| Connects to a fan. A bad connection will disable the fan, causing a fan error. |
CN1101| Used for debugging, and is for technical service purpose only. | FPC 40 way, .50mm 
CN1102| Connects to the DMD board and main board. A bad connection will cause a blank screen. |



## Links

 - http://www.cnx-software.com/2012/07/18/building-linux-kernel-3-0-8-for-telechips-tcc8925-mini-pcs-cx-01-z900-tizzbird-n1/
