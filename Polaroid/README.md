Its SOC is a RockChip chip ``RK2818``.

The model seems to be the ``MIDCF43PR001`` and the firmware should be [here](http://www.southerntelecom.com/polaroidsupport/firmware/)

```
$ lsusb 
Bus 001 Device 126: ID 0bb4:0c02 HTC (High Tech Computer Corp.) Dream / ADP1 / G1 / Magic / Tattoo (Debug)
```

If you (with the device powered off) press ``Home`` and insert the usb cable then

```
$ lsusb 
Bus 001 Device 080: ID 2207:281a  
```

In this [thread](http://forum.xda-developers.com/showthread.php?t=1286305) they talk about ``rkflashtool``
that I downloaded from this repository (exist several version, this is the first search result from github)

    https://github.com/neo-technologies/rkflashtool

In this repositoy seems to exist also some documentation about the protocol!

Now I can compile and launching it I obtain

```
$ ./rkflashtool v
rkflashtool: info: rkflashtool v5.2
rkflashtool: info: Detected RK2818...
rkflashtool: info: interface claimed
rkflashtool: info: chip version: 281X-2010.01.01-V100
```

for the chip version [RK2818](http://www.rock-chips.com/a/en/products/RK28_Series/2013/0730/331.html), then I can ask for the flash version

```
$ ./rkflashtool n
rkflashtool: info: rkflashtool v5.2
rkflashtool: info: Detected RK2818...
rkflashtool: info: interface claimed
rkflashtool: info: Flash ID: ec d5 84 72 50
rkflashtool: info: Flash Info:
	Manufacturer: Samsung (0)
	Flash Size: 2048MB
	Block Size: 1024KB
	Page Size: 8KB
	ECC Bits: 24
	Access Time: 36
	Flash CS: <0>
```

The boot parameters are

```
$ ./rkflashtool p
rkflashtool: info: rkflashtool v5.2
rkflashtool: info: Detected RK2818...
rkflashtool: info: interface claimed
rkflashtool: info: reading parameters at offset 0x00000000
rkflashtool: info: size:  0x000001fb
FW_VERSION: 1.0.0
MAGIC: 0x5041524B
ATAG: 0x60000800
MACHINE: 1616
CHECK_MASK: 0x80
KERNEL_IMG: 0x60008000
COMBINATION_KEY: F,0,1
CMDLINE: noinitrd console=ttyS1,115200n8n init=/init root=/dev/mtdblock2 mem=202M@0x60000000 mtdparts=rk28xxnand:0x00002000@0x00002000(misc),0x00004000@0x00004000(kernel),0x00002000@0x00008000(boot),0x00004000@0x0000A000(recovery),0x00060000@0x0000E000(system),0x00066000@0x0006E000(backup),0x0003A000@0x000D4000(cache),0x00080000@0x0010E000(userdata),-@0x0018E000(user)
```

from which I can obtain the name of the partition: below I dump the ``system`` image

```
$ ./rkflashtool r system > system.img
rkflashtool: info: rkflashtool v5.2
rkflashtool: info: Detected RK2818...
rkflashtool: info: interface claimed
rkflashtool: info: working with partition: system
rkflashtool: info: found offset: 0x0000e000
rkflashtool: info: found size: 0x00060000
rkflashtool: info: reading flash memory at offset 0x0006dfe0... Done!
```

and with the ``cramfsprogs`` package I decompress it in a directory

```
$ sudo cramfsck system.img -x system/
$ tree system/ -L 1
/opt/my-devices/system/
├── app
├── bin
├── build.prop
├── build.prop.bak
├── etc
├── fonts
├── framework
├── lib
├── media
├── tts
├── usr
└── xbin
```


## Components

 - [K9GAG08U0E](http://n2k1.com/n2k1/NB7/PDF/K9GAG08U0E.pdf) Flash memory
 - 2x [H5PS1G63EFR](https://www.skhynix.com/eolproducts.view.do?pronm=DDR2+SDRAM&srnm=H5PS1G63EFR&rk=18&rc=graphics)
 - Audio [ALC5621](https://www.11h.net/blog/wp-content/uploads/2010/08/ALC5621.pdf)
 - Power management [PT1502](https://www.icware.ru/pdf/0000945.pdf)
 - [MMA7660FC](https://www.nxp.com/docs/en/data-sheet/MMA7660FC.pdf) accelerometer (labelling: three rows ``263``, ``7660``, ``3JSM``)
 - [PCF8563](https://www.nxp.com/docs/en/data-sheet/PCF8563.pdf) real-time clock/calendar
 - [XPT2046](https://www.buydisplay.com/download/ic/XPT2046.pdf) touch screen controller

## Link

 - http://linux-rockchip.info/
 - Openembedded [layer page](http://layers.openembedded.org/layerindex/branch/master/layer/meta-rockchip/)
