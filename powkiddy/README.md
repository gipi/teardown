# Powkiddy X3/J6

|     |                 |
|-----|-----------------|
| Cpu | Actions ATS3603 (128 pins seems like the ATJ2273B with an ePAD) |
| Memory | 64 MB DDR SDRAM Hynix HY5DU121622CTP |
| Display | 4.3 "IPS screen (800x480) |
| Battery | 1200 mah (3.7V) internal non-removable battery |
| Audio | 3.5 headphone jack (Stereo) input, built-in mono speaker. Microphone |
| USB | MiniUSB |
| Internal storage | 16MB 25Q2128FV SPI NOR flash |
| External storage | micro SD |
| Controls | Stick left analog, D-Pad, buttons A, B, X, Y, L, R, Start, Select, Vol (+), Vol (-), ON / OFF |
| OS | Free Open Source microC / OS (as the Powkiddy X16 / X19 consoles) |
| Emulators | CPS1, CPS2, MVS, MD (without SVP support), SNES (without special chips), NES, GB / GBC and GBA. Master System, Game Gear and PC Engine a through the GBA scene |

## Reversing

The SoC used for this device has not publicly available information, so
reverse engineering is the only way to know about its internal design.

 - [BROM](BROM.md)
 - [booting process](mbrec.md)

## Serial

The ``UART1_TX`` signal is at pin 128, this pin is not connected to anything so
soldering skills are needed to connect a wire (or you can use a micro probe);
it's possible to see a message when enters ``ADFU`` mode (baudrate 115200)

```
[BOOT] Jun  8 2012 14:58:27 brec_entry.c module_entry 283
[BOOT] brec_entry.c module_entry
[Storage] (SNOR) Drv entry
[BOOT] boot_size = 1024
SPINOR ID ----------------: 0xef.40.18
[BOOT] nand inithw ok
[BOOT] nand boot's page size 0x1 in sectors
SPINOR ID ----------------: 0xef.40.18
[BOOT]
```

[This code](uart.c) outputs the ``ASCII`` charset from the serial (the ``adfu``
python module is in the [``meta/actions/``](../meta/actions/) subdirectory of this repo):

```
$ make uart.bin.encrypted
mips-linux-gnu-gcc -march=4kec -EL -Wall -fno-plt -fno-pic -c uart.c
mips-linux-gnu-ld -EL -Tadfu.ld startup.o uart.o -o uart.elf
mips-linux-gnu-objcopy -O binary uart.elf uart.bin
../meta/actions/adfuload encrypt uart.bin > uart.bin.encrypted
[info]: original size: 1072 rounded size: 1536
[info]: encrypting binary
rm uart.bin
$ python3 -m adfu.exe --device 10d6:10d6  --binary uart.bin.encrypted
ERROR - usb.libloader:libloader.py:127 - Libusb 1 (/usr/lib/libusb-1.0.so) could not be loaded
Traceback (most recent call last):
  File ".local/lib/python3.9/site-packages/usb/libloader.py", line 116, in load_library
    return ctypes.CDLL(lib)
  File "/usr/lib/python3.9/ctypes/__init__.py", line 374, in __init__
    self._handle = _dlopen(self._name, mode)
OSError: /usr/lib/libusb-1.0.so: cannot open shared object file: No such file or directory
ERROR - usb.backend.libusb1:libusb1.py:948 - Error loading libusb 1.0 backend
INFO - usb.core:core.py:1260 - find(): using backend "usb.backend.libusb1"
INFO:usb.core:find(): using backend "usb.backend.libusb1"
INFO:__main__:uploading binary 'uart.bin.encrypted' and executing it
 [+] ADECadfus
INFO:adfu.cbw.app:ADECadfus
 [+] uploading
INFO:adfu.cbw.app:uploading
3072it [00:00, 785185.98it/s]
 [+] executing at b4060000
INFO:adfu.cbw.app:executing at b4060000
```

## USB

If you connect the device powered on via the mini USB port you obtain the
following

```
[1878108.599416] usb 2-10: new high-speed USB device number 11 using xhci_hcd
[1878108.748188] usb 2-10: New USB device found, idVendor=10d6, idProduct=1101, bcdDevice= 1.00
[1878108.748199] usb 2-10: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[1878108.748205] usb 2-10: Product: media-player
[1878108.748209] usb 2-10: Manufacturer: actions
[1878108.748213] usb 2-10: SerialNumber: ㈱㐳㘵㠷㜸㔶㌴ㄲ
[1878108.752525] usb-storage 2-10:1.0: USB Mass Storage device detected
[1878108.752740] scsi host6: usb-storage 2-10:1.0
[1878109.803429] scsi 6:0:0:0: Direct-Access     actions  media-player     0100 PQ: 0 ANSI: 0 CCS
[1878109.804201] sd 6:0:0:0: Attached scsi generic sg3 type 0
[1878109.804460] sd 6:0:0:0: Power-on or device reset occurred
[1878109.819909] sd 6:0:0:0: [sdd] Attached SCSI removable disk
```

If instead you keep pressed the volume up button on startup you obtain

```
[1878212.732670] usb 2-10: new high-speed USB device number 12 using xhci_hcd
[1878212.881140] usb 2-10: New USB device found, idVendor=10d6, idProduct=10d6, bcdDevice= 1.00
[1878212.881152] usb 2-10: New USB device strings: Mfr=0, Product=0, SerialNumber=0
```

that seems to be the ADFU mode.

## Q700

It exists another device very similar that is the ``Powkiddy x16``, named also
``Q700`` that has an upgrade firmware and it seems to use the ``ATJ2279B``.

It's possible to transform the ``Q700.fw`` into ``Q700.afi`` using the
``atjboottool`` from the Rockbox project.

What is obtained is an ``sqlite3`` database containing files(?)

```
sqlite> select Filename, printf('%08x', NumberB), printf('%08x', NumberC) from ADFUS;
ADFUS.BIN|ffffffffc0000000|00000000
adec09_1.bin|ffffffffb4060000|00000000
```

You can dump the file in the following way

```
$ sqlite3 Q700.afi 'select quote(File) from FileTable WHERE FileName LIKE "adec09_1.bin"' | xxd -r -p > Q700/adec09_1.bin
```

and also decrypt it

```
$ ../meta/actions/adfuload decrypt Q700/adec09_1.bin > Q700/adec09_1.bin_decrypted
```

Analyzing this module is possible to dump the internal memory using [this script](Q700/q700.py).
