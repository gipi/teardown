# USB mode

There are three USB mode present in different ways in this device

## Boot mode

This is the mode inside the SoC and is the failsafe for all the boot modes available;
in the specification is described in the following way:

 1. The TCC8900 makes internal SRAM area starts from zero, and copies USB interrupt
    service routine to internal SRAM area.
 2. It waits until USB connection is established.
 3. Once it is connected, host transfers first the parameter for USB loader routine including
    start address, destination address and the amount of data to be transferred (with a unit of
    packet).
 4. The TCC8900 starts communicating between a host PC with fixed amount of data which
    is called as packet. The packet size of TCC8900 is 512 bytes.
 5. At every successful reception of packet, it copies them where the destination address
    pointed, and after all amount of data has been copied, it starts program where the start
    address pointed


It's similar of what is described in the datasheet of [TCC760](https://www.bg-electronics.de/datenblaetter/Schaltkreise/TCC760.pdf)
or that other people [have found](https://dreamlayers.blogspot.com/2013/03/telechips-tcc76x-usb-boot.html); it loads via the [tcctool](https://github.com/Rockbox/rockbox/blob/master/utils/tcctool/tcctool.c)
some code in ``SDRAM`` and executes it.

To boot in this mode the SoC checks some GPIOs, namely ``BM[2:0]``, ``GPIOE[3]`` etc... but since I have not
the board layout (yet :P) I don't know where the physical pull-up/pull-down resistor are placed (I supposed
this configuration is done by using resistor); however, in the specification is indicated the boot ROM as the
failsafe when the other configurations fail.

With this in mind I simply shorted the ``CE`` and ``VCC`` of the NAND Flash to obtain the following
USB device

```
Feb 23 10:35:23 turing kernel: [ 1038.755195] usb 2-2: new high-speed USB device number 3 using xhci_hcd
Feb 23 10:35:23 turing kernel: [ 1038.903570] usb 2-2: config 1 interface 0 altsetting 0 bulk endpoint 0x82 has invalid maxpacket 64
Feb 23 10:35:23 turing kernel: [ 1038.903576] usb 2-2: config 1 interface 0 altsetting 0 bulk endpoint 0x1 has invalid maxpacket 64
Feb 23 10:35:23 turing kernel: [ 1038.903582] usb 2-2: New USB device found, idVendor=140e, idProduct=b077
Feb 23 10:35:23 turing kernel: [ 1038.903586] usb 2-2: New USB device strings: Mfr=0, Product=0, SerialNumber=0
Feb 23 10:35:40 turing kernel: [ 1056.322886] usb 2-2: USB disconnect, device number 3
```

```
Bus 002 Device 004: ID 140e:b077 Telechips, Inc.
Device Descriptor:
  bLength                18
  bDescriptorType         1
  bcdUSB               1.10
  bDeviceClass            0 (Defined at Interface level)
  bDeviceSubClass         0
  bDeviceProtocol         0
  bMaxPacketSize0        64
  idVendor           0x140e Telechips, Inc.
  idProduct          0xb077
  bcdDevice            1.00
  iManufacturer           0
  iProduct                0
  iSerial                 0
  bNumConfigurations      1
  Configuration Descriptor:
    bLength                 9
    bDescriptorType         2
    wTotalLength           32
    bNumInterfaces          1
    bConfigurationValue     1
    iConfiguration          0
    bmAttributes         0xc0
      Self Powered
    MaxPower              100mA
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        0
      bAlternateSetting       0
      bNumEndpoints           2
      bInterfaceClass       255 Vendor Specific Class
      bInterfaceSubClass    255 Vendor Specific Subclass
      bInterfaceProtocol    255 Vendor Specific Protocol
      iInterface              0
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x82  EP 2 IN
        bmAttributes            2
          Transfer Type            Bulk
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0040  1x 64 bytes
        bInterval               0
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x01  EP 1 OUT
        bmAttributes            2
          Transfer Type            Bulk
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0040  1x 64 bytes
        bInterval               0
Device Status:     0x2300
  (Bus Powered)
```

To try to fuzz this mode I imported the ``tcctool`` from the rockbox project and adapted the code for
this SoC.

## FWDN mode

This is the "normal" firmware upgrade mode, done by the bootloader, ``tccboot``.

This is a bootloader mode that allows to upgrade the firmware using special crafted images;
maybe is activated pressing the button nearer the sdcard slot: see the image below

![](Images/boot.jpg)

when this happens the device has the red led on and the
usb device has identifier ``140e:b077``(like the ``vid:pid`` used for ``TCC91XX`` [here](https://github.com/JeffreyLau/JJWD-K8_icsCream/blob/a9790f6edf973d9e6b102f9be89c7b7f883f1cb2/bootable/bootloader/lk/platform/tcc_shared/include/usb/usbdev_class.h)). 

## SCSI mode

This is the ``/var/log/messages`` when is booting and a USB cable is attacched

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

```
$ lsusb -v
 ...
Bus 002 Device 010: ID 140e:b058 Telechips, Inc. 
Device Descriptor:
  bLength                18
  bDescriptorType         1
  bcdUSB               2.00
  bDeviceClass            0 (Defined at Interface level)
  bDeviceSubClass         0 
  bDeviceProtocol         0 
  bMaxPacketSize0        64
  idVendor           0x140e Telechips, Inc.
  idProduct          0xb058 
  bcdDevice            3.ff
  iManufacturer           1 TELECHIPS
  iProduct                2 File-backed Storage Gadget
  iSerial                 3 0123456789ABCDEF0123456789ABCDEF
  bNumConfigurations      1
OTG Descriptor:
  bLength                 3
  bDescriptorType         9
  bmAttributes         0x03
    SRP (Session Request Protocol)
    HNP (Host Negotiation Protocol)
  Configuration Descriptor:
    bLength                 9
    bDescriptorType         2
    wTotalLength           35
    bNumInterfaces          1
    bConfigurationValue     1
    iConfiguration          4 Self-powered
    bmAttributes         0xc0
      Self Powered
    MaxPower                2mA
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        0
      bAlternateSetting       0
      bNumEndpoints           2
      bInterfaceClass         8 Mass Storage
      bInterfaceSubClass      6 SCSI
      bInterfaceProtocol     80 Bulk-Only
      iInterface              5 Mass Storage
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x81  EP 1 IN
        bmAttributes            2
          Transfer Type            Bulk
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0200  1x 512 bytes
        bInterval               0
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x01  EP 1 OUT
        bmAttributes            2
          Transfer Type            Bulk
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0200  1x 512 bytes
        bInterval               1
 ...
```

```
$ sudo sginfo /dev/sg2
INQUIRY response (cmd: 0x12)
----------------------------
Device Type                        0
Vendor:                    TELECHIP
Product:                   MASS STORAGE
Revision level:            03ff
$ sudo sg_inq /dev/sg2
standard INQUIRY:
  PQual=0  Device_type=0  RMB=1  LU_CONG=0  version=0x00  [no conformance claimed]
  [AERC=0]  [TrmTsk=0]  NormACA=0  HiSUP=0  Resp_data_format=1
  SCCS=0  ACC=0  TPGS=0  3PC=0  Protect=0  [BQue=0]
  EncServ=0  MultiP=0  [MChngr=0]  [ACKREQQ=0]  Addr16=0
  [RelAdr=0]  WBus16=0  Sync=0  [Linked=0]  [TranDis=0]  CmdQue=0
    length=36 (0x24)   Peripheral device type: disk
 Vendor identification: TELECHIP
 Product identification: MASS STORAGE    
 Product revision level: 03ff
```

