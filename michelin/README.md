# Michelin

I named this after the brand on it.

https://4pda.ru/forum/index.php?showtopic=708093&st=3080

## Processor

It's an infamous ``AM7331`` related to **Actions Microelectronics**, like
the MiraScreen.

It seems that the Philips SPF4080P photo frame has the same SoC.

https://fccid.io/YL5SPF4080P/Label/Label-Sample-1341109

## Flash

K9F2G08U0M

## Bootlog

There is a pad on the back with a ``TX`` label on the side and if you
connect a serial at 115200 baud you obtain the following
messages on power up:

```
***AL1203, AM12030002,brec.bin Ver  svn7663 Dec 22 2014 ,Flash:2***

MU_REG1 0xb01c0080,0x2600
MU_REG2 0xb01c0090,0x0

======== Flash Init hardware==========
CMU_COREPLL: 86a90df
CMU_BUSCLK :     390
SDR_CTL:   f0120
SDR_INIT :c0000014
SDR_TIMING : c92814a
SDR_AUTORFC      4fc
SDR_EN:       1
SDR_MODE :      32
SDR_PD: 2625a00
SDR_INITD:    cf08
***end3,4,27500 Khz,48901****


## boot  Flash ID:0x1580daec#
ECC BCH8 
ecctl:0x1 

RECLAIM_VAL:6 bitECC,set:6 bit
###6,4,330Mhz,15MHz,15714Khz ###
###6,4,48900###

#PHY_TMPBUF(NandDevInfo.PhyCache):0xa00c0010 
#GPageBuf:0xa00c0a20 NandDevInfo.TestCache:0xa00c1a30,logical Page:4
ScanSearchZoneTbls: [SCAN] Found valid zone table 0 for die 0 at block 8, page 8.
ScanSearchZoneTbls: [SCAN] Found valid zone table 0 for die 1 at block 8, page 8.

===NO:    0,Read Zone : 0, Blk:8,Pge:8===
###CodeSize:4300,1 ###
##1. Reclaim: BCH 6,6 ##brecsize:128 4000
##2. Super:1, Readclaim:1
##3. Super:0, Wearleveling:0
##4.ECC_reclaim,1,overecc:0
##5.DataBlk:984,MultiPlane:0
##6.PageCache:0xa00c0010,4
##7.NandDevInfo.PhyCache:0xa00c1020:
FlashBuf:a00c2990,:a00c2ba0
FlashBuf_S:a00c2db0,D:a00c3dc0

+++1> SystemSize:4300, 1:4300,2:20,3:4300,4:0, 5:0,6:0,7:6e6e0 +++


#get file :0,res:1#
file:nand:EASY    BIN
get file filenum:3,result:0xxx
rd
ENter BY GPIO  255
ret = 0
system_code_check end 883ms
system_code_check retrun 0
###MBRC_Whole:3, code:2,55aa2d66!####
### Read 55aa2d66,55aa2d66,3
### Read 55aa2d66,55aa2d66,3
### Read 55aa2d66,55aa2d66,3
mbrc_test_repair OK
brec_test_repair OK
 mmu_num_mods: 0x7f00000
 mmu_min_virt_addr: 0x100000
 mmu_region_phy_addr: 0x54000
 mmu_region_size: 0xc0000
 vma_flash_drv_start: 0x80014ce0
 vma_flash_drv_end: 0xcdc0
 sys_heap_start: 0x80021aa0
 sys_heap_size: 0x32560
 mmm_heap_start: 0x80054000
 mmm_heap_size: 0x0
SD_KINIT_BIN
SDRAMLoadAddr =0xa01c1000
BREC#kinit.bin is loaded at a01c0000
BREC#syscfg.sys is loaded at a0000000
BREC#nand648.drv is loaded at a0014ce0
./../../../nand_driver_entry.cBrec nand exit!! 
Starting Kernel at 0x801c0000
fb_drv.c 1284 BL_CTL=[0x8201e853]
global_data.show_backlight:0
act_setUsbStat:stat=0
FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSinf[FS_Fat_OpenDir]:747 ,cd = 1

check U16     BIN:
check U24     BIN:
check U32     BIN:
CheckOver
FSinf[FS_Fat_OpenDir]:747 ,cd = 1

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSinf[FS_Fat_OpenDir]:747 ,cd = 1

head_temp->figure:1,head_temp->format:.MP4
the 0 num head_array name is C:\0\1.MP4
FSinf[FS_Fat_OpenDir]:747 ,cd = 1

FSinf[FS_Fat_OpenDir]:747 ,cd = 1

FSinf[FS_Fat_OpenDir]:747 ,cd = 1

init_meap(80200000,100000)
!!!!1203act_dac_init
line:891,video_name:C:\0\1.MP4
MMM_VIDEO: load_plugin(avd_mov.lib)

===NO:    1,Read Zone : 1, Blk:8,Pge:8===
video type 13
support
MMM_VIDEO: load_plugin(ad_audio.lib)
MMM_VIDEO: load_plugin(vd_xvid.lib)
here
dpb_num:[4],ow[800],oh[480],fmt[3],reload[1],lvds[0]
dpb, start video, init_flg=1
open speaker 
reset da module
GetGPIO(30):0
adc_key is d,global_data.open_show_bat:1
osd_api.c 701 x+w =[842]
[O_O]check your low crystal!
GetGPIO(30):0
adc_key is d,global_data.open_show_bat:2
osd_api.c 701 x+w =[842]
set standby_enable
GetGPIO(30):0
adc_key is d,global_data.open_show_bat:3
osd_api.c 701 x+w =[842]
GetGPIO(30):0
```


It's present a USB connector on the side, if you insert a USB key and
press the first button from the top at power up you obtain the following log:

```
***AL1203, AM12030002,brec.bin Ver  svn7663 Dec 22 2014 ,Flash:2***

MU_REG1 0xb01c0080,0x2600
MU_REG2 0xb01c0090,0x0

======== Flash Init hardware==========
CMU_COREPLL: 86a90df
CMU_BUSCLK :     390
SDR_CTL:   f0120
SDR_INIT :c0000014
SDR_TIMING : c92814a
SDR_AUTORFC      4fc
SDR_EN:       1
SDR_MODE :      32
SDR_PD: 2625a00
SDR_INITD:    cf08
***end3,4,27500 Khz,48901****


## boot  Flash ID:0x1580daec#
ECC BCH8 
ecctl:0x1 

RECLAIM_VAL:6 bitECC,set:6 bit
###6,4,330Mhz,15MHz,15714Khz ###
###6,4,48900###

#PHY_TMPBUF(NandDevInfo.PhyCache):0xa00c0010 
#GPageBuf:0xa00c0a20 NandDevInfo.TestCache:0xa00c1a30,logical Page:4
ScanSearchZoneTbls: [SCAN] Found valid zone table 0 for die 0 at block 8, page 20.
ScanSearchZoneTbls: [SCAN] Found valid zone table 0 for die 1 at block 8, page 8.

===NO:    0,Read Zone : 0, Blk:8,Pge:20===
###CodeSize:4300,1 ###
##1. Reclaim: BCH 6,6 ##brecsize:128 4000
##2. Super:1, Readclaim:1
##3. Super:0, Wearleveling:0
##4.ECC_reclaim,1,overecc:0
##5.DataBlk:984,MultiPlane:0
##6.PageCache:0xa00c0010,4
##7.NandDevInfo.PhyCache:0xa00c1020:
FlashBuf:a00c2990,:a00c2ba0
FlashBuf_S:a00c2db0,D:a00c3dc0

+++1> SystemSize:4300, 1:4300,2:20,3:4300,4:0, 5:0,6:0,7:6e6e0 +++


#get file :0,res:1#
file:nand:EASY    BIN
get file filenum:3,result:0xxx
rd
ENter BY GPIO  255
ret = 0
system_code_check end 883ms
system_code_check retrun 0
###MBRC_Whole:3, code:2,55aa2d66!####
### Read 55aa2d66,55aa2d66,3
### Read 55aa2d66,55aa2d66,3
### Read 55aa2d66,55aa2d66,3
mbrc_test_repair OK
brec_test_repair OK
 mmu_num_mods: 0x7f00000
 mmu_min_virt_addr: 0x100000
 mmu_region_phy_addr: 0x54000
 mmu_region_size: 0xc0000
 vma_flash_drv_start: 0x80014ce0
 vma_flash_drv_end: 0xcdc0
 sys_heap_start: 0x80021aa0
 sys_heap_size: 0x32560
 mmm_heap_start: 0x80054000
 mmm_heap_size: 0x0
SD_KINIT_BIN
SDRAMLoadAddr =0xa01c1000
BREC#kinit.bin is loaded at a01c0000
BREC#syscfg.sys is loaded at a0000000
BREC#nand648.drv is loaded at a0014ce0
./../../../nand_driver_entry.cBrec nand exit!! 
Starting Kernel at 0x801c0000
fb_drv.c 1284 BL_CTL=[0x8201e853]
global_data.show_backlight:0
fb_drv.c 1284 BL_CTL=[0x82030933]
send KEY_LONG,k=8
check U16     BIN:
check U24     BIN:
check U32     BIN:
CheckOver
[O_O]check your low crystal!
set standby_enable
enter a_fsm
a_plug_process:902
act_setUsbStat:stat=0
plug:4033

[Kernel]task[22],stack=[c6ffc:c6000]
act_setUsbStat:stat=0
set_vbus_detect_threshold:old val-c00
set_vbus_detect_threshold:set val-c10
set_vbus_detect_threshold:new val-c10
AI:state from 00 ->00
AOTG uoc driver probe successfully

[Kernel]task[1],stack=[d0ffc:d0000]
debounce:total 120ms stable 120ms status 0x101
usb_alloc_dev:sem a
__hotplug_event:msg=11

[Kernel]task[10],stack=[e0ffc:e0000]
FSinf[FS_Dev_Register]:907 ,FS_Dev_Register dev:00917294

ep->maxp=512
ep->maxp=512
test-disk: Write Protect is off
test-disk: Mode Sense: 45 00 00 08
__hotplug_event:msg=88
FSinf[FS_Os_ParseDev]:268 ,FS Has MBR.

FSinf[FS_Os_ParseMbr]:652 ,DBR addr toobig(2048)
FSinf[FS_Os_ParseBpb]:395 ,FS dev:0x10100000

FSinf[FS_Os_ParseBpb]:461 ,FS Clustsize:4096 BytePerSec:512 SecPerClust:8 perSec:1

FSinf[FS_Os_ParseBpb]:539 ,FS FAT32 
FSinf[FS_Os_ParseBpb]:541 ,TotalClust:979843

Start checking upgrade firmware version......
FSerr[FS_Fat_act_OpenFile]:n:
ACTUPGRADE.BIN
41 43 54 55 50 47 52 41 44 45 2e 42 49 4e 00 
Open D:\ACTUPGRADE.BIN , file pointer = 0x0
card install errorFSerr[FS__fat_FindFilePath]:dname_start == NULL 


ilePath]:FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\0\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

d_OpenDir err 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\1\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\2\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\3\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\4\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

width_bar:0
FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\0\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\1\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\2\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\3\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\4\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\0\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\1\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\2\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\3\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSinf[FS_Fat_OpenDir]:747 ,cd = 0

FSerr[FS_Fat_OpenDir]:cdchild n:
D:\4\
FSinf[FS_OpenDir]:597 ,FS_Fat_OpenDir err 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

width_bar:0
fb_drv.c 1284 BL_CTL=[0x82030853]
act_setUsbStat:stat=0
FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSinf[FS_Fat_OpenDir]:747 ,cd = 1

FSinf[FS_Fat_OpenDir]:747 ,cd = 1

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSerr[FS__fat_FindFilePath]:dname_start == NULL 

FSinf[FS_Fat_OpenDir]:747 ,cd = 1

head_temp->figure:1,head_temp->format:.MP4
the 0 num head_array name is C:\0\1.MP4
msg.msg.data[0]:88
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
FSinf[FS_Fat_OpenDir]:747 ,cd = 1

msg.msg.data[0]:c8
msg.msg.data[0]:c8
FSinf[FS_Fat_OpenDir]:747 ,cd = 1

FSinf[FS_Fat_OpenDir]:747 ,cd = 1

init_meap(80200000,100000)
!!!!1203act_dac_init
line:891,video_name:C:\0\1.MP4
MMM_VIDEO: load_plugin(avd_mov.lib)

===NO:    1,Read Zone : 1, Blk:8,Pge:8===
video type 13
support
MMM_VIDEO: load_plugin(ad_audio.lib)
MMM_VIDEO: load_plugin(vd_xvid.lib)
here
dpb_num:[4],ow[800],oh[480],fmt[3],reload[1],lvds[0]
dpb, start video, init_flg=1
open speaker 
reset da module
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
msg.msg.data[0]:c8
GetGPIO(30):0

```

## ADFU mode

It's possible to enter ``ADFU`` mode shorting the ``!CE`` pin
of the flash with 3.3V

https://web.archive.org/web/20080327212622/http://www.s1mp3.org/en/docs_deadrec.php

https://wenku.baidu.com/view/543c8b4d852458fb770b56ee.html

```
kernel: [1217438.375421] usb 2-6: new high-speed USB device number 49 using xhci_hcd
kernel: [1217438.907401] usb 2-6: new full-speed USB device number 50 using xhci_hcd
kernel: [1217438.927629] usb 2-6: not running at top speed; connect to a high speed hub
kernel: [1217438.927799] usb 2-6: New USB device found, idVendor=1de1, idProduct=1205, bcdDevice= 1.00
kernel: [1217438.927801] usb 2-6: New USB device strings: Mfr=0, Product=0, SerialNumber=0
mtp-probe: checking bus 2, device 50: "/sys/devices/pci0000:00/0000:00:14.0/usb2/2-6"
mtp-probe: bus: 2, device: 50 was not an MTP device
mtp-probe: checking bus 2, device 50: "/sys/devices/pci0000:00/0000:00:14.0/usb2/2-6"
mtp-probe: bus: 2, device: 50 was not an MTP device
```
