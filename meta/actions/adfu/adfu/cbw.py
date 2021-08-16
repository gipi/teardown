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
from pathlib import Path
from tqdm import tqdm
from hexdump import hexdump
from .utils import argparse_vendor_product

USB_wMaxPacketSize = 0x200  # TODO: extract info from USB itself


#logging.basicConfig()
usb_logger = logging.getLogger("usb")
# usb_logger.setLevel(logging.DEBUG)
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

import usb.backend.libusb1
usb_backend = usb.backend.libusb1.get_backend(find_library=lambda x: "/usr/lib/libusb-1.0.so")


def usb_conf(idVendor, idProduct):
    '''
    The endpoint is a unidirectional and data can be transferred either in an
    IN or OUT direction. Bulk IN endpoint is used to read data from the device
    to the host and bulk OUT endpoint is used to send data from the host to the
    device.
    '''
    logger.debug(f'trying to connect to {idVendor:x}:{idProduct:x}')
    dev = usb.core.find(idVendor=idVendor, idProduct=idProduct, backend=usb_backend)  # try to avoid 253th find() error

    if not dev:
        return None, None, None

    logger.debug(f'found device {dev!r}')

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

    logger.debug(f'using interface for read {ep_r}')
    logger.debug(f'using interface for write {ep_w}')

    return dev, ep_r, ep_w


# FIXME: this is going to be the final general purpose cbw-write-to-device
def cbw_write_(interface, cmd, tag, size, arg0, arg1=None, subCmd=0x0, flags=0x00, subCmd2=0x0, cmdLength=0x10):
    arg1 = arg1 if arg1 else size
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

    cbw = bytes.fromhex(cbw_fmt.replace('\n', ' '))

    logger.debug(cbw)

    interface.write(cbw)

    return cbw


# USE THIS!!!
def cbw_send(interface, cmd, transferLength, arg0, arg1, arg2, tag=0x0, flags=0x0, cmdLength=0x10):
    tag_hex = struct.pack('I', tag).hex()
    tranferLength_hex = struct.pack('I', transferLength).hex()
    flags = struct.pack('B', flags).hex()
    cmdLength_hex = struct.pack('B', cmdLength).hex()

    cmd_hex = struct.pack('B', cmd).hex()
    arg0_hex = struct.pack('I', arg0).hex()
    arg1_hex = struct.pack('I', arg1).hex()
    arg2_hex = struct.pack('I', arg2).hex()

    cbw_fmt = f'''55 53 42 43
    {tag_hex}
    {tranferLength_hex}
    {flags}
    00
    {cmdLength_hex}
    {cmd_hex} {arg0_hex}
    {arg1_hex}
    {arg2_hex} 00 00 00'''
    logger.debug(cbw_fmt)

    cbw = bytes.fromhex(cbw_fmt.replace('\n', ' '))

    logger.debug(cbw)

    interface.write(cbw)

    return cbw


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
    if size != 0x0d:
        logger.warning(f"{size=} is not compliant")

    response = endpoint.read(size)
    logger.debug(f'CBW response (len={len(response)}): {bytearray(response)}')

    # unpack it as a normal CSW packet ignoring the "size" parameter
    signature, responseTag, residue, status, = struct.unpack('4sIIB', response[:0x0d])

    if signature != b'USBS':
        raise ValueError(f'wrong signature: {signature}')

    if tag and responseTag != tag:
        raise ValueError(f'response with tag {responseTag:x} instead of {tag:x}')

    if status != 0:
        raise ValueError(f'CSW status={status:x}')

    logger.debug(f'data residue={residue:x}')


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
    cbw_write_(e_write, 0x05, 0x88, stat.st_size, address, stat.st_size, 0x0000)

    count = 0
    progress = tqdm(total=stat.st_size)
    with open(path, 'rb') as firmware:
        while count < stat.st_size:
            content = firmware.read(USB_wMaxPacketSize)
            e_write.write(content)
            count += USB_wMaxPacketSize
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

    logger.info(f'hwsc_get_size response={response}')

    size = struct.unpack('H', response)[0]
    cbw_read_response(r, 0xa79210)

    return size


def hwsc_get_info(r, w, size):
    logger.info('hwsc get info')
    cbw_write(w, 0xb0, 0x77c05c94, 0x10, 0x00, size << 16, 0xc480)
    response = r.read(size)

    logger.info(f'hwsc_get_info response={response}')
    logger.info('\n' + hexdump(response, result='return'))

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


def ADECadfus(path, r, w, address=0xb4040000):
    '''
    cmd 05 tag 88 address b4040000 subCmd 00 -> upload
    cmd 10 tag 00 address b4040000 subCmd 00 -> execute
    '''
    logger.info('ADECadfus')
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


def flash_dump(r, w, output):
    logger.info('dump flash')
    size = 0x80000
    cbw_write_(w, 0xb0, 0xcafebabe, size, 0x00, 0x00, subCmd=0x2e9f)

    size_packet = r.wMaxPacketSize
    with output.open(mode='wb') as fout:
        while size > 0:
            response = r.read(size_packet)
            logger.debug('\n' + hexdump(response, result='return'))
            fout.write(response)
            size -= size_packet

    cbw_read_response(r, 0xcafebabe)


def smth_dump(r, w, path_output):
    '''Try cmd 6 with arg0 equal to the size of the dump'''
    logger.info('dump something from address 0xa00080000')
    size = 0x20000
    cbw_write_(w, 0x06, 0xcafebabe, size, 0x00, 0x00)

    size_packet = r.wMaxPacketSize
    with path_output.open(mode='wb') as file_out:
        while size > 0:
            response = r.read(size_packet)
            logger.debug('\n' + hexdump(response, result='return'))
            file_out.write(response)
            size -= size_packet

    cbw_read_response(r, 0xcafebabe)


def cmd06_dump(r, w, size, path_output):
    '''From disassembly seems that this command returns something
    from address a0080000

        if (cmd == 6) {
            usb_otg->OTG_OUT1CS = 0;
            usb_send((dword *)&DAT_a0080000,usb_otg->OTG_FIFO1DAT,usb_otg);
        }
    '''
    logger.info(f'dump something with command 06 using size {size:x}')

    cbw_write_(w, 0x06, 0xcafebabe, size=size, arg0=size, arg1=0)

    size_packet = r.wMaxPacketSize
    with path_output.open(mode='wb') as file_out:
        while size > 0:
            response = r.read(size_packet)
            logger.debug('\n' + hexdump(response, result='return'))
            file_out.write(response)
            size -= size_packet

    cbw_read_response(r, 0xcafebabe)


def _6f80_dump(r, w):
    logger.info('dump something from sub command 6f80')
    size_packet = r.wMaxPacketSize
    cbw_write_(w, 0xb0, 0xcafebabe, size=10, arg0=size_packet, arg1=0,subCmd=0x6f80)

    response = r.read(size_packet)
    logger.info('\n' + hexdump(response, result='return'))

    cbw_read_response(r, 0xcafebabe)


def brc_dump(r, w):
    logger.info('dumping brc ???')
    cbw_write_(w, 0xb0, 0xffffffff, 0x0200, 0x00, 0x00, subCmd=0x6f36)
    response = r.read(0x200)

    logger.info('\n' + hexdump(response, result='return'))
    cbw_read_response(r, 0xffffffff)


def mbrc_dump(r, w):
    logger.info('dumping mbrc')
    cbw_write_(w, 0xb0, 0xffffffff, 0x0200, 0x00, 0x00, subCmd=0x6f37)
    response = r.read(0x200)

    logger.info('\n' + hexdump(response, result='return'))
    cbw_read_response(r, 0xffffffff)


def mbr_dump(r, w):
    logger.info('dump MBR')
    tag = 0x88
    cbw_write_(w, 0x08, tag, 0x200, 0x7000, 0x10000, 0x80, 0x8000)
    response = r.read(0x200)

    logger.info('\n' + hexdump(response, result='return'))
    cbw_read_response(r, tag)


def FW_dump(r, w):
    '''
    0040   55 aa f2 0f 31 32 38 30 30 2e 35 35 30 34 2e 73   U...12800.5504.s
    0050   64 6b 00 00 00 00 00 00 e1 1d 01 11 34 0f ec 26   dk..........4..&
    0060   61 63 74 69 6f 6e 73 00 00 00 00 00 00 00 00 00   actions.........
    0070   61 63 74 69 6f 6e 73 00 00 00 00 00 00 00 00 00   actions.........
    0080   61 63 74 69 6f 6e 73 00 00 00 00 00 00 00 00 00   actions.........
    0090   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    00a0   6d 65 64 69 61 20 70 6c 61 79 65 72 00 00 00 00   media player....
    00b0   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    00c0   80 47 00 00 00 00 00 00 00 00 00 00 00 00 00 00   .G..............
    00d0   00 70 00 00 00 00 00 00 00 00 00 00 00 00 00 00   .p..............
    00e0   00 d8 03 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    00f0   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    0100   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    0110   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    0120   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    0130   46 77 3a 32 30 31 34 2d 31 31 2d 31 37 00 00 00   Fw:2014-11-17...
    0140   47 45 4e 45 52 49 43 00 55 53 42 20 44 49 53 4b   GENERIC.USB DISK
    0150   20 44 45 56 49 43 45 00 31 2e 30 30 00 00 00 00    DEVICE.1.00....
    0160   30 03 55 00 53 00 42 00 20 00 4d 00 41 00 53 00   0.U.S.B. .M.A.S.
    0170   53 00 20 00 53 00 54 00 4f 00 52 00 41 00 47 00   S. .S.T.O.R.A.G.
    0180   45 00 20 00 43 00 4c 00 41 00 53 00 53 00 00 00   E. .C.L.A.S.S...
    0190   1b 41 63 74 69 6f 6e 73 20 4d 69 63 72 6f 45 6c   .Actions MicroEl
    01a0   74 72 6f 6e 69 63 73 20 4c 74 64 2e 00 00 00 00   tronics Ltd.....
    01b0   00 17 41 63 74 69 6f 6e 73 20 41 4d 37 58 20 44   ..Actions AM7X D
    01c0   50 46 20 44 65 76 69 63 65 00 00 00 00 00 00 00   PF Device.......
    01d0   00 00 0a 76 32 2e 30 30 2e 30 30 30 30 00 00 00   ...v2.00.0000...
    01e0   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    01f0   00 00 00 00 e1 1d 00 22 00 00 00 00 00 00 00 00   ......."........
    0200   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    0210   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    0220   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
    0230   00 00 00 00 00 00 00 00 00 00 00 00 00 00 f8 6f   ...............o
    '''
    logger.info('FW generic usb disk device')
    dump_from(r, w, 0x0000, 0x20000)


def dump_from(r, w, start, size):
    logger.debug(f'dump from {start:x} for {size:x}')
    tag = 0x88
    cbw_write_(w, 0x08, tag, size, start, arg1=size << 7, subCmd=0x80, subCmd2=0x8000)
    response = r.read(size)

    logger.info('\n' + hexdump(response, result='return'))
    cbw_read_response(r, tag)


def dump_native_mbr(r, w):
    dump_from(r, w, 0x7000, 0x200)


def upload_adfus(r, w, path_fw):
    '''This initializes the board and activates the command other than 05 and 10'''
    path_adec = path_fw / 'ADECadfus'
    path_adfu = path_fw / 'ADFUadfus'
    ADECadfus(path_adec, r, w)
    ADFUadfus(path_adfu, r, w)


def disconnect(r, w):
    cbw_write(w, 0xb0, 0x38f688, 0x10, 0x00, 0x00, 0xb586)
    cbw_read_response(r, 0x38f688)


def usage(progname):
    print(f'''usage: {progname} --device=idv:idp <adfu directory>''')
    sys.exit(1)


def old_main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "rd:h", [
            'device=',
            'help',
            'logger-usb=',
            'logger-app=',
            'dump=',
            'dump-smth=',
            'dump-cmd06=',
            'reset'
        ])
    except getopt.GetoptError as err:
        print(err)
        usage(sys.argv[0])
        sys.exit(1)

    id_vendor, id_product = None, None

    dump_flash = False
    dump_smth = False
    dump_cmd06 = False
    reset = False

    for option, value in opts:
        if option in ('-d', '--device'):
            v, p = value.split(':')
            id_vendor = int(v, 16)
            id_product = int(p, 16)
        elif option in ('--logger-app',):
            print(f'LOGGING APP set to {value}')
            logger.setLevel(value)
        elif option in ('--logger-usb',):
            print(f'LOGGING USB set to {value}')
            usb_logger.setLevel(value)
        elif option in ('--reset',):
            pass
        elif option in ('--dump-smth',):
            dump_smth = Path(value)
        elif option in ('--dump',):
            dump_flash = Path(value)
        elif option in ('--dump-cmd06',):
            dump_cmd06 = Path(value)
        elif option in ('-r', '--reset'):
            reset = True
        elif option in ('-h', '--help'):
            usage(sys.argv[0])
            sys.exit(1)

    if not id_vendor or not id_product:
        print('please indicate the vendor and product id', file=sys.stderr)
        sys.exit(2)

    path_adfus = Path(args[0])  # you must pass a directory containing at least ADECadfu and ADFUadfu

    # configure the endpoint for the bulk transfers
    dev, endpoint_read, endpoint_write = usb_conf(id_vendor, id_product)
    if not dev:
        logger.critical('no device found')
        sys.exit(2)
    # upload and execute the ADECadfus (from the serial we see test ddr stuffs)
    upload_adfus(endpoint_read, endpoint_write, path_adfus)

    hwsc(path_adfus / 'HWSChwsc', endpoint_read, endpoint_write)
    fwsc(path_adfus / 'F648fwsc', endpoint_read, endpoint_write)

    # isActions(endpoint_read, endpoint_write)
    # isSwitchToAdfu(endpoint_read, endpoint_write)
    # getSysInfo(endpoint_read, endpoint_write)

    # mbrc/brc order must be preserved
    mbrc_dump(endpoint_read, endpoint_write)
    brc_dump(endpoint_read, endpoint_write)
    if dump_flash:
        flash_dump(endpoint_read, endpoint_write, dump_flash)

    if dump_smth:
        smth_dump(endpoint_read, endpoint_write, dump_smth)

    FW_dump(endpoint_read, endpoint_write)
    dump_native_mbr(endpoint_read, endpoint_write)
    if dump_cmd06:
        cmd06_dump(endpoint_read, endpoint_write, 0xf8000, dump_cmd06)

    disconnect(endpoint_read, endpoint_write)


def args_parse():
    import argparse
    import functools

    parser = argparse.ArgumentParser(
        description='''swiss knife tool to create CBW packet on the fly

    55 53 42 43 signature
    xx xx xx xx tag
    yy yy yy yy transferLength
    ww          flags
    zz          LUN
    1f          cmdLength
    <cmd>       cmd
    AA AA AA AA arg0
    BB BB BB BB arg1
    CC CC CC CC arg2
    kk kk       padding
        ''',
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument(
        '--device',
        type=argparse_vendor_product,
        required=True,
        help="vendor:product of the device you want to interact with")
    parser.add_argument('--cmd', type=functools.partial(int, base=0), required=True)
    parser.add_argument('--size', type=functools.partial(int, base=0), required=True)
    parser.add_argument('--arg0', type=functools.partial(int, base=0), required=True)
    parser.add_argument('--arg1', type=functools.partial(int, base=0), required=True)
    parser.add_argument('--arg2', type=functools.partial(int, base=0), required=True)
    parser.add_argument('--expected-response',
        type=functools.partial(int, base=0),
        help='is expected to have a response of X bytes',
    )
    parser.add_argument('--expected-csw-size',
        type=functools.partial(int, base=0),
        help='is expected to have a CSW response of X bytes',
        default=0xd,
    )

    return parser.parse_args()


if __name__ == '__main__':
    args = args_parse()

    # configure the endpoint for the bulk transfers
    dev, endpoint_read, endpoint_write = usb_conf(*args.device)
    if not dev:
        logger.critical('no device found')
        sys.exit(2)

    _cbw_write(endpoint_write, args.cmd, args.size, args.arg0, args.arg1, args.arg2)

    if (args.expected_response):
        response_data = endpoint_read.read(args.expected_response)
        logger.info('response:\n' + hexdump(response_data, result='return'))

    try:
        cbw_read_response(endpoint_read, size=args.expected_csw_size)
    except usb.USBError as e:
        if e.backend_error_code == usb.backend.libusb1.LIBUSB_ERROR_OVERFLOW:
            logger.info(
                f"the endpoint sent more bytes that you requested (try to modify {args.expected_csw_size=})")
        else:
            logger.info(f"encountered {e.backend_error_code=}")
            raise e
