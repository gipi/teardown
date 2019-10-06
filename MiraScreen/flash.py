#!/usr/bin/env python3
'''
Application to interact with the ADFU mode of a MiraScreen device.

Links for some example of USB data transfer flow:

    https://mvidner.blogspot.com/2017/01/usb-communication-with-python-and-pyusb.html
    https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/usb-bulk-and-interrupt-transfer

Below the serial console output

GPIO 74 is press88888888test ddr
msdly:00-----fail
msdly:01-----fail
msdly:02-----fail
msdly:03-----fail
msdly:04-----fail
msdly:05-----fail
msdly:06-----fail
msdly:07-----fail
msdly:08-----fail
msdly:09-----fail
msdly:0a-----fail
msdly:0b-----fail
msdly:0c-----fail
msdly:0d-----fail
msdly:0e-----fail
msdly:0f-----fail
msdly:10-----fail
msdly:11-----ok
msdly:12-----ok
msdly:13-----ok
msdly:14-----ok
msdly:15-----ok
msdly:16-----ok
msdly:17-----ok
msdly:18-----ok
msdly:19-----ok
msdly:1a-----ok
msdly:1b-----ok
msdly:1c-----fail
msdly:1d-----fail
msdly:1e-----fail
final msdly:16
msldyreg:51614
USE_UART = 1

INIT_HWSC:****hwsc.bin Ver Apr 25 2017 Tim:11:32:49,hard_ware_scan_main****
++++++FOR_TEST CMU_NANDCLK :50a


 nand_flash_inithw_HWSC,run to:144
 CMU_COREPLL 0xb0010000:388d94b
 CMU_BUSCLK 0xb001000c:2000038c
 SDR_CLKDLY_VAL 0xB0070024 :0
 SDR_CTL 0xB0070000:30166
 SDR_INIT 0xB0070024:0
 SDR_TIMING 0xB0070030:25aa7377
 SDR_AUTORFC 0xB0070014 d00
 SDR_EN 0xB0070008:1
 SDR_MODE 0xB0070018 :521
 SDR_PD 0xB0070034:0
 SDR_INITD 0xB0070028 :13880
 Cell Type        is:2
 Page Size(KB)    is:2
 Block Size(KB)   is:128
 NAND Flash       is:1580f198
 CMD92       is:0
 BootDiskType  F648, length=0x0050
 CHIPINFO: 0x1101000e
 Dram size is:0x8000000
 Test size is:0x7f80000
 current addr:0x5a5aa5a5 PC:0xa00188a4 SP:0xa000af84
 current addr:0x5a5aa5a5 PC:0x800188b4 SP:0xa000af84
 start data line test pattern :0xaaaaaaaa
 finish pattern:0xaaaaaaaa
 start data line test pattern :0x55555555
 finish pattern:0x55555555

 ***AL1213 AM1101,1101,fwsc.bin Ver Apr 25 2017 Tim:11:32:50*****

 MU_REG1:0x00003e00
 MU_REG2:0x00003e00
 CMU_COREPLL [b0010000]: 388d94b
 CMU_BUSCLK [b001000c]: 2000038c
 SDR_CTL [b0070000]: 30166
 SDR_CMD [b007000c]: 0
 SDR_TIMING [b0070030]: 25aa7377
 SDR_RFSH [b0070014]: d00
 SDR_EN [b0070008]: 1
 SDR_MODE [b0070018]: 521
 SDR_CLKDLY [b007005c]: 10a0000
 SDR_MSTDLY [b0070060]: 51713
 SDR_LATENCY [b0070058]: 8000605
 --------- getcorepll() COREPLL val:492
##Set nand Flash:32800 Khz##
##CMU_COREPLL:0x388d94b ,NAND_CTL: 0x48901
###CMU_NANDCLK:0x50e,CLKCTL:0x9,NANDCTL48901###
###NandDMAChannel:0 ###
###4.User_Data_2Byte:0
###5.EIP_IRQ_MODE:1
###6.TEST_RB_CTL:0
##7.SPECIAL_DMA_Test:0
##8.PageBCH24 Read:0,Write:0
##9.PageBCH8 Read:0,Write:0
##10.pseudoNoise:0
 mode32:0x608401a4,mode8:0x608001a4
 mode32:0x1a46084,mode8:0x1a46080
 [INIT] INIT_CreateBlkTbls() enter.

 ====22 boot nand Flash CHIPID:0x1580f198 ====
 Flash ChipNo:0 CHIPID:1580f198
 Flash ChipNo:1 CHIPID:0

 ===Set:0x0, NAND_ECCCTL:0x1,
 AM1213 Set ECC type BCH8
 NAND_ECCCTL:0x1 ==

 --------- getcorepll() COREPLL val:492
 CMU_COREPLL:0x388d94b
###7,9,492Mhz,20MHz,20500Khz ###
###Set BMA:82Mhz###
###CMU_NANDCLK:0x507,CLKCTL:0x9,NANDCTL48900###

 INIT_DBG: ================NAND STORAGE INFOR================
 INIT_DBG:     ChipEnableInfo is:0x1
 INIT_DBG:     ChipCnt        is:0x1
 INIT_DBG:     BankCntPerChip is:0x1
 INIT_DBG:     DieCntPerChip  is:0x1
 INIT_DBG:     PlaneCntPerDie is:0x1
 INIT_DBG:     SectorNumPerPage  is:0x4
 INIT_DBG:     PageNumPerPhyBlk  is:0x40
 INIT_DBG:     TotalBlkNumPerDie is:0x400
 INIT_DBG:     OperationOpt is:0x0
 INIT_DBG:     FrequencePar is:0x14
 INIT_DBG:     Reclaim is:6
 INIT_DBG:     SpareSize is:128
 INIT_DBG:     NandChipId   is:0x1580f198
 INIT_DBG: ====================================================

 INIT_DBG: ================LOGIC ORGANIZE INFO=================
 INIT_DBG:     DataBlkNumPerZone  is:0x3d8
 INIT_DBG:     PageNumPerLogicBlk is:0x40
 INIT_DBG:     SectorNumPerLogicPage is:0x4
 INIT_DBG:     ZoneNumPerDie      is:0x1
 INIT_DBG: ====================================================

 INIT_DBG: ===============SPECICAL CMMOND INFO=================
 INIT_DBG:     MultiProgCmd         is:0x11, 0x80
 INIT_DBG:     MultiCopyReadCmd     is:0x0, 0x0, 0x30
 INIT_DBG:     MultiCopyProgCmd     is:0x8c, 0x11, 0x8c
 INIT_DBG:     MultiBlkAddrOffset   is:0x0
 INIT_DBG:     BadBlkFlagPst        is:0x0
 INIT_DBG:     ReadMultiOpStatusCmd is:0x71
 INIT_DBG:     InterChip0StatusCmd  is:0x70
 INIT_DBG:     InterChip1StatusCmd  is:0x70
 INIT_DBG: =====================================================

 INIT_DBG: ===============Sector Bimmap=================
 INIT_DBG 1.===>Sector_BitmapBBF:0x3
 INIT_DBG 2.===>Sector_BitmapSpare:0x7
 INIT_DBG 3.===>USER_DataByte:0x2
 INIT_DBG 4.===>USER_SectorCnt:0x3
 INIT_DBG 5.===>TOTAL_SPAREDATA:6
 INIT_DBG 6.===>Single_BitmapSpare:7
 INIT_DBG: =====================================================

 TOTAL_BANKS          1
 DIENUM_IN_FLASH      1
 ZONENUM_PER_DIE      1
 SUBLK_PER_DIE        1024
 PAGES_PER_SUBLK      64
 MULTI_PLANE_SUPPORT  0
 DIE_FIRST_SUBLK_NUM  8
###NandDevInfo.PhyCache :a0032640 ,size:2048
###GPageBuf:0xa0031630
 [SCAN] ScanSearchZoneTbls: (DieNo: 0) enter.absZoneTblStartPage:28
 ScanSearchZoneTbls: [SCAN] Found valid zone table 0 for die 0 at block 8, page 28.
 [SCAN] ScanSearchZoneTbls: (DieNo: 0) out, ret 0
 [INIT] INIT_CreateBlkTbls() out, ret 0.
##1.Reclaim: 6,6,brecsize:128KB
##2.Read Super:1, Readclaim:1
##3.Write Super:0, Wearleveling:1
##4.ECC_reclaim,1,overecc:1
##5.DataBlk:984,MultiPlane:0
##6.PageCache:0xa0031630,4
##7.NandDevInfo.PhyCache:0xa0032640:
 INIT_CreateBlkTbls end
 logical_whole_cap: 3d800,123MB
 fwsc BlocNo:4,checksum:0,brecSize128KB
 writing finished(4MB)OP:1,0x     0,0

 afcfl fwsc n!
'''
import time
import os
import sys
import logging
import struct
import usb
from tqdm import tqdm
from hexdump import hexdump


logging.basicConfig()
logging.getLogger("usb").setLevel(logging.DEBUG)
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)

# WARNING: this script has been tested only with this device
#          change these at your own risk!!!
ID_VENDOR = 0x1de1
ID_PRODUCT = 0x1205


def usb_conf():
    '''
    The endpoint is a unidirectional and data can be transferred either in an
    IN or OUT direction. Bulk IN endpoint is used to read data from the device
    to the host and bulk OUT endpoint is used to send data from the host to the
    device.
    '''
    dev = usb.core.find(idVendor=ID_VENDOR, idProduct=ID_PRODUCT)

    if not dev:
        raise ValueError(f'Device not found {ID_VENDOR:x}:{ID_PRODUCT:x}')

    logger.info(f'found device {dev!r}')

    dev.set_configuration()
    cfg = dev.get_active_configuration()
    intf = cfg[(0, 0)]

    ep_r = usb.util.find_descriptor(
        intf,
        custom_match=lambda e:
            usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_IN)

    ep_w = usb.util.find_descriptor(
        intf,
        custom_match=lambda e:
            usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_OUT)

    logger.info(f'using interface for read {ep_r}')
    logger.info(f'using interface for write {ep_w}')

    return ep_r, ep_w


def cbw_write_(interface, cmd, tag, size, arg0, arg1, subCmd, cmdLength=0x10):
    cmd_hex = struct.pack('B', cmd).hex()
    tag_hex = struct.pack('I', tag).hex()
    cmdLength_hex = struct.pack('B', cmdLength).hex()
    arg0_hex = struct.pack('I', arg0).hex()
    arg1_hex = struct.pack('I', arg1).hex()
    size_hex = struct.pack('I', size).hex()
    subCmd_hex = struct.pack('H', subCmd).hex()

    cbw_fmt = f'''55 53 42 43
    {tag_hex}
    {size_hex}
    00
    00
    {cmdLength_hex}
    {cmd_hex} {arg0_hex}
    {arg1_hex}
    {subCmd_hex} 00 00 00 00 00'''
    logger.debug(cbw_fmt)

    cbw = bytearray.fromhex(cbw_fmt.replace('\n', ' '))

    logger.debug(cbw)

    interface.write(cbw)


def cbw_write(interface, cmd, tag, cmdLength, dstAddr, size, subCmd):
    cmd_hex = struct.pack('B', cmd).hex()
    tag_hex = struct.pack('I', tag).hex()
    cmdLength_hex = struct.pack('B', cmdLength).hex()
    dstAddr_hex = struct.pack('I', dstAddr).hex()
    size_hex = struct.pack('I', size).hex()
    subCmd_hex = struct.pack('H', subCmd).hex()

    cbw_fmt = f'''55 53 42 43
    {tag_hex}
    {size_hex}
    00
    00
    {cmdLength_hex}
    {cmd_hex} {dstAddr_hex}
    {size_hex}
    {subCmd_hex} 00 00 00 00 00'''
    logger.debug(cbw_fmt)

    cbw = bytearray.fromhex(cbw_fmt.replace('\n', ' '))

    logger.debug(cbw)

    interface.write(cbw)


def cbw_read_response(endpoint, tag=None):
    '''5.2 Command Status Wrapper (CSW)

    The CSW shall start on a packet boundary and shall end as a short packet
    with exactly 13 (0Dh) bytes transferred. Fields appear aligned to byte
    offsets equal to a multiple of their byte size. All CSW transfers shall be
    ordered with the LSB (byte 0) first (little endian).'''
    response = endpoint.read(0x0d)
    logger.debug(response)

    signature, responseTag, residue, status = struct.unpack('4sIIB', response)

    if signature != b'USBS':
        raise ValueError('wrong signature: {signature}')

    if tag and responseTag != tag:
        raise ValueError(f'response with tag {responseTag:x} instead of {tag:x}')

    if status != 0:
        raise ValueError(f'CSW status={status:x}')

    logger.info(f'data residue={residue:x}')


def upload(path, e_read, e_write, address, checkTag=True):
    logger.info('uploading')
    stat = os.stat(path)
    logger.debug(f'size={stat.st_size}')
    cbw_write(e_write, 0x05, 0x88, 0x10, address, stat.st_size, 0x0000)

    count = 0
    progress = tqdm(total=stat.st_size)
    with open(path, 'rb') as firmware:
        while count < stat.st_size:
            content = firmware.read(64)
            e_write.write(content)
            count += 64
            progress.update(count)

    progress.close()

    cbw_read_response(e_read, tag=0x88 if checkTag else None)


def execute_adfus(r, w, address, subCmd):
    logger.info(f'executing at {address:08x}')
    cbw_write(w, 0x10, 0x00, 0x00, address, 0x00, subCmd)
    cbw_read_response(r, 0x00)


def execute(r, w, address, subCmd):
    logger.info(f'executing at {address:08x}')
    cbw_write(w, 0xb0, 0x00, 0x10, address, 0x00, subCmd)
    cbw_read_response(r, 0x00)


def hwsc_get_size(r, w):
    logger.info('hwsc get size')
    cbw_write(w, 0xb0, 0xa79210, 0x10, 0x00, 0x00020000, 0x6aae)
    response = r.read(2)

    logger.debug(f'hwsc_get_size response={response}')

    size = struct.unpack('H', response)[0]
    cbw_read_response(r, 0xa79210)

    return size


def hwsc_get_info(r, w, size):
    logger.info('hwsc get info')
    cbw_write(w, 0xb0, 0x77c05c94, 0x10, 0x00, size << 16, 0xc480)
    response = r.read(size)

    logger.debug(f'hwsc_get_info response={response}')
    logger.debug('\n' + hexdump(response, result='return'))

    cbw_read_response(r, 0x77c05c94)


def hwsc(path, r, w):
    '''
    It stands for "hardware scan" (from the log)

    cmd 05 tag 88       subCmd 00    -> upload
    cmd b0 tag 00       subCmd 4658  -> execute
    cmd b0 tag a79210   subCmd 6aae  -> get size
    cmd b0 tag 77c05c94 subCmd c480  -> get info
    '''
    logger.info('HWSC')
    address = 0xa0010000
    upload(path, r, w, address)
    execute(r, w, address, 0x4658)
    size = hwsc_get_size(r, w)
    hwsc_get_info(r, w, size)


def fwsc(path, r, w):
    '''
    It stands for firmware scan'''
    logger.info('FWSC')
    address = 0xa0018000
    upload(path, r, w, address)
    execute(r, w, address, 0x4658)
    logger.info('waiting for scan to complete execution')
    time.sleep(5)  # we must wait for the code to execute
    size = hwsc_get_size(r, w)
    hwsc_get_info(r, w, size)


def ADECadfus(path, r, w):
    '''
    cmd 05 tag 88 address b4040000 subCmd 00 -> upload
    cmd 10 tag 00 address b4040000 subCmd 00 -> execute
    '''
    logger.info('ADECadfus')
    address = 0xb4040000
    # from the USB dump seems that the tag is not correctly returned in this case
    upload(path, r, w, address, checkTag=False)
    execute_adfus(r, w, address, 0x00)


def ADFUadfus(path, r, w):
    '''
    cmd 05 tag 88 address a0000000 subCmd 00 -> upload
    cmd 10 tag 00 address a0000000 subCmd 00 -> execute
    '''
    logger.info('ADFUadfus')
    address = 0xa0000000
    upload(path, r, w, address)
    execute_adfus(r, w, address, 0x00)


def flash_dump(r, w):
    logger.info('dump flash')
    cbw_write_(w, 0xb0, 0xffffffff, 0x0200, 0x00, 0x00, 0x6f37)
    response = r.read(0x200)

    logger.debug('\n' + hexdump(response, result='return'))
    cbw_read_response(r, 0xffffffff)


def mbrc_dump(r, w):
    logger.info('dumping mbrc')
    cbw_write_(w, 0xb0, 0xffffffff, 0x0200, 0x00, 0x00, 0x6f36)
    response = r.read(0x200)

    logger.debug('\n' + hexdump(response, result='return'))
    cbw_read_response(r, 0xffffffff)


def disconnect(r, w):
    cbw_write(w, 0xb0, 0x38f688, 0x10, 0x00, 0x00, 0xb586)
    cbw_read_response(r, 0x38f688)


def usage(progname):
    print(f'''usage: {progname} ADEC ADFU hwsc fwsc''')
    sys.exit(1)


if __name__ == '__main__':
    if len(sys.argv) < 5:
        usage(sys.argv[0])

    path_adec = sys.argv[1]
    path_adfu = sys.argv[2]
    path_hwsc = sys.argv[3]
    path_fwsc = sys.argv[4]

    # configure the endpoint for the bulk transfers
    endpoint_read, endpoint_write = usb_conf()
    # upload and execute the ADECadfus (from the serial we see test ddr stuffs)
    ADECadfus(path_adec, endpoint_read, endpoint_write)
    ADFUadfus(path_adfu, endpoint_read, endpoint_write)
    hwsc(path_hwsc, endpoint_read, endpoint_write)
    fwsc(path_fwsc, endpoint_read, endpoint_write)
    flash_dump(endpoint_read, endpoint_write)
    mbrc_dump(endpoint_read, endpoint_write)

    disconnect(endpoint_read, endpoint_write)
