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

'''
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


def usage(progname):
    print(f'''usage: {progname} ADEC ADFU hwsc''')
    sys.exit(1)


if __name__ == '__main__':
    if len(sys.argv) < 4:
        usage(sys.argv[0])

    path_adec = sys.argv[1]
    path_adfu = sys.argv[2]
    path_hwsc = sys.argv[3]

    # configure the endpoint for the bulk transfers
    endpoint_read, endpoint_write = usb_conf()
    # upload and execute the ADECadfus (from the serial we see test ddr stuffs)
    ADECadfus(path_adec, endpoint_read, endpoint_write)
    ADFUadfus(path_adfu, endpoint_read, endpoint_write)
    hwsc(path_hwsc, endpoint_read, endpoint_write)
