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

As usual not a lot is known about the internal of this SOC, some
progress about [reversing its boot process](mbrec.md) are in the way.

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
