It's possible to use [rkflashtool](https://github.com/neo-technologies/rkflashtool)
in order to flash this table once you have the *recovery mode*:
you have to press ``Vol+`` button, insert the usb connector, wait an usb device to appear
into the computer log and then release the button.

Then we obtain the chip type: **RK2928**

```
$. /rkflashtool v
rkflashtool: info: rkflashtool v5.2
rkflashtool: info: Detected RK2928...
rkflashtool: info: interface claimed
rkflashtool: info: chip version: 292X-2012.08.23-V300
```

with 4GB of flash

```
$ ./rkflashtool n
rkflashtool: info: rkflashtool v5.2
rkflashtool: info: Detected RK2928...
rkflashtool: info: interface claimed
rkflashtool: info: Flash ID: 2c 68 04 4a a9
rkflashtool: info: Flash Info:
	Manufacturer: Micron (4)
	Flash Size: 4096MB
	Block Size: 1024KB
	Page Size: 4KB
	ECC Bits: 24
	Access Time: 32
	Flash CS: <0>
```

The boot parameters

```
./rkflashtool p
rkflashtool: info: rkflashtool v5.2
rkflashtool: info: Detected RK2928...
rkflashtool: info: interface claimed
rkflashtool: info: reading parameters at offset 0x00000000
rkflashtool: info: size:  0x0000025d
FIRMWARE_VER:4.1.1
MACHINE_MODEL:TB-071MC
MACHINE_ID:007
MANUFACTURER:RK2928SDK
MAGIC: 0x5041524B
ATAG: 0x60000800
MACHINE: 2928
CHECK_MASK: 0x80
KERNEL_IMG: 0x60408000
#RECOVER_KEY: 1,1,0,20,0
CMDLINE:console=ttyFIQ0 androidboot.console=ttyFIQ0 init=/init initrd=0x62000000,0x00800000 mtdparts=rk29xxnand:0x00002000@0x00002000(misc),0x00004000@0x00004000(kernel),0x00008000@0x00008000(boot),0x00010000@0x00010000(recovery),0x00020000@0x00020000(backup),0x00040000@0x00040000(cache),0x00200000@0x00080000(userdata),0x00002000@0x00280000(kpanic),0x00130000@0x00282000(system),-@0x003b2000(user)
```

shows all the partitions present on the flash; dumping the system partition

```
$ ./rkflashtool r system > system.img
rkflashtool: info: rkflashtool v5.2
rkflashtool: info: Detected RK2928...
rkflashtool: info: interface claimed
rkflashtool: info: working with partition: system
rkflashtool: info: found offset: 0x00282000
rkflashtool: info: found size: 0x00130000
rkflashtool: info: reading flash memory at offset 0x003b1fe0... Done!
$ file /opt/teardown/tec_tb-071mc/system.img
system.img: Linux rev 1.0 ext3 filesystem data, UUID=bb9e0a8d-c7b7-4c9a-81f9-8021a9d7c625, volume name "system"
```

gives us an ``ext3`` filesystem.
