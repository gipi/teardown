This is the pico projector for Samsung

Product page: http://www.samsung.com/it/support/model/SP0351VBX/EN

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
