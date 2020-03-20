#!/usr/bin/env python3
'''
Application to interact with the ADFU mode of an Actions device.

This has been tested only on a couple of devices, primarly the MiraScreen
with AM8252 chip (ID_VENDOR:ID_PRODUCT = 0x1de1:0x1205)


# Actions ADFU mode

SUBSYSTEMS=="usb", ACTION=="add", ATTRS{idVendor}=="ide1", ATTRS{idProduct}=="1205", GROUP="plugdev", MODE="0660"
'''
import getopt
import time
import os
import sys
import logging
import struct
import usb
from tqdm import tqdm
from hexdump import hexdump


#logging.basicConfig()
usb_logger = logging.getLogger("usb")
usb_logger.setLevel(logging.DEBUG)
logger = logging.getLogger(f'{__name__}.app')  # application level logger
logger.setLevel(logging.DEBUG)
app_stream = logging.StreamHandler()
low_stream = logging.StreamHandler()

app_formatter = logging.Formatter(' [+] %(message)s')
low_formatter = logging.Formatter('%(levelname)s - %(name)s:%(filename)s:%(lineno)d - %(message)s')

app_stream.setFormatter(app_formatter)
low_stream.setFormatter(low_formatter)
logger.addHandler(app_stream)
usb_logger.addHandler(low_stream)


def usb_conf(idVendor, idProduct):
    '''
    The endpoint is a unidirectional and data can be transferred either in an
    IN or OUT direction. Bulk IN endpoint is used to read data from the device
    to the host and bulk OUT endpoint is used to send data from the host to the
    device.
    '''
    logger.info(f'trying to connect to {idVendor:x}:{idProduct:x}')
    dev = usb.core.find(idVendor=idVendor, idProduct=idProduct)

    if not dev:
        raise ValueError(f'Device not found {idVendor:x}:{idProduct:x}')

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


# FIXME: this is going to be the final general purpose cbw-write-to-device
def cbw_write_(interface, cmd, tag, size, arg0, arg1=0x00, subCmd=0x0, flags=0x00, subCmd2=0x0, cmdLength=0x10):
    tag_hex = struct.pack('I', tag).hex()
    size_hex = struct.pack('I', size).hex()
    flags = struct.pack('B', flags).hex()
    cmdLength_hex = struct.pack('B', cmdLength).hex()

    cmd_hex = struct.pack('B', cmd).hex()
    arg0_hex = struct.pack('I', arg0).hex()
    arg1_hex = struct.pack('I', arg1).hex()
    subCmd_hex = struct.pack('H', subCmd).hex()
    subCmd2_hex = struct.pack('H', subCmd2).hex()

    cbw_fmt = f'''55 53 42 43
    {tag_hex}
    {size_hex}
    {flags}
    00
    {cmdLength_hex}
    {cmd_hex} {arg0_hex}
    {arg1_hex}
    {subCmd_hex} {subCmd2_hex} 00 00 00'''
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


def cbw_read_response(endpoint, tag=None, size=0x0d):
    '''5.2 Command Status Wrapper (CSW)

    The CSW shall start on a packet boundary and shall end as a short packet
    with exactly 13 (0Dh) bytes transferred. Fields appear aligned to byte
    offsets equal to a multiple of their byte size. All CSW transfers shall be
    ordered with the LSB (byte 0) first (little endian).'''
    response = endpoint.read(size)
    logger.debug(f'CBW response (len={len(response)}): {response}')

    signature, responseTag, residue, status, = struct.unpack('4sIIB', response)

    if signature != b'USBS':
        raise ValueError(f'wrong signature: {signature}')

    if tag and responseTag != tag:
        raise ValueError(f'response with tag {responseTag:x} instead of {tag:x}')

    if status != 0:
        raise ValueError(f'CSW status={status:x}')

    logger.info(f'data residue={residue:x}')


def isSwitchToAdfu(e_read, e_write):
    '''CDrm::IsSwitchToAdfu()'''
    logger.info('is switch to adfu?')
    cbw_write_(
        e_write,
        cmd=0xcb,
        tag=0x00,
        size=0x02,
        flags=0x80,
        cmdLength=0xc,
        arg0=0x21,
        arg1=0x00020000,
    )
    response = e_read.read(0x02)
    logger.info('response:\n' + hexdump(response, result='return'))
    cbw_read_response(e_read, tag=None)


def isActions(e_read, e_write):
    '''Routine reversed from FWBurningTool, libProduction.so'''
    logger.info('is actions?')
    cbw_write_(
        e_write,
        cmd=0xcc,
        tag=0x00,
        size=0x12,
        flags=0x80,
        arg0=0x00,
        arg1=0x00120000,
    )
    response = e_read.read(0x12)
    logger.info('isActions:\n' + hexdump(response, result='return'))
    cbw_read_response(e_read, tag=None)


def switch(e_read, e_write, addr):  # more like execute
    '''AM7331 CDrm::Switch() in libProduction.so'''
    logger.info('switch')
    cbw_write_(
        e_write,
        cmd=0x10,
        tag=0x00,
        size=0x00,
        arg0=addr,
    )
    cbw_read_response(e_read, tag=None)


def getSysInfo(e_read, e_write):
    '''AM7331 CDrm::GetSysInfo() in libProduction.so'''
    logger.info('sysinfo')
    cbw_write_(
        e_write,
        cmd=0xaf,
        tag=0x00,
        size=0x200,
        flags=0x80,
        arg0=0x00,
        arg1=0x02000000,
    )
    response = e_read.read(0x200)
    logger.info('response:\n' + hexdump(response, result='return'))
    cbw_read_response(e_read, tag=None)


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
    cbw_read_response(r, tag=0x00)


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


def mbr_dump(r, w):
    logger.info('dump MBR')
    tag = 0x88
    cbw_write_(w, 0x08, tag, 0x200, 0x7000, 0x10000, 0x80, 0x8000)
    response = r.read(0x200)

    logger.debug('\n' + hexdump(response, result='return'))
    cbw_read_response(r, tag)


def disconnect(r, w):
    cbw_write(w, 0xb0, 0x38f688, 0x10, 0x00, 0x00, 0xb586)
    cbw_read_response(r, 0x38f688)


def usage(progname):
    print(f'''usage: {progname} --device=idv:idp''')
    sys.exit(1)


if __name__ == '__main__':
    try:
        opts, args = getopt.getopt(sys.argv[1:], "d:h", [
            'device=',
            'help',
        ])
    except getopt.GetoptError as err:
        print(err)
        usage(sys.argv[0])
        sys.exit(1)

    id_vendor, id_product = None, None

    for option, value in opts:
        if option in ('-d', '--device'):
            v, p = value.split(':')
            id_vendor = int(v, 16)
            id_product = int(p, 16)
        elif option in ('-h', '--help'):
            usage(sys.argv[0])
            sys.exit(1)

    if not id_vendor or not id_product:
        print('please indicate the vendor and product id', file=sys.stderr)
        sys.exit(2)

    # configure the endpoint for the bulk transfers
    endpoint_read, endpoint_write = usb_conf(id_vendor, id_product)
    # upload and execute the ADECadfus (from the serial we see test ddr stuffs)

    isActions(endpoint_read, endpoint_write)
    # isSwitchToAdfu(endpoint_read, endpoint_write)
    getSysInfo(endpoint_read, endpoint_write)

    disconnect(endpoint_read, endpoint_write)
