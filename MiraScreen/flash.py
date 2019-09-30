#!/usr/bin/env python3
'''
Application to interact with the ADFU mode of a MiraScreen device.
'''
import os
import sys
import logging
import struct
import usb
from tqdm import tqdm


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


def cbw_write(interface, cmd, tag, cmdLength, dstAddr, size):
    cmd_hex = struct.pack('B', cmd).hex()
    tag_hex = struct.pack('I', tag).hex()
    cmdLength_hex = struct.pack('B', cmdLength).hex()
    dstAddr_hex = struct.pack('I', dstAddr).hex()
    size_hex = struct.pack('I', size).hex()

    cbw_fmt = f'''55 53 42 43
    {tag_hex}
    {size_hex}
    00
    00
    {cmdLength_hex}
    {cmd_hex} {dstAddr_hex}
    {size_hex} 00
    00 00 00 00 00 00'''
    logger.debug(cbw_fmt)

    cbw = bytearray.fromhex(cbw_fmt.replace('\n', ' '))

    logger.debug(cbw)

    interface.write(cbw)

    logger.debug('successfull writing')


def upload(path, e_read, e_write):
    stat = os.stat(path)
    cbw_write(e_write, 0x05, 0x88, 0x10, 0xa0010000, stat.st_size)

    count = 0
    progress = tqdm(total=stat.st_size)
    with open(path, 'rb') as firmware:
        while count < stat.st_size:
            content = firmware.read(64)
            e_write.write(content)
            count += 64
            progress.update(count)

    progress.close()

    response = e_read.read(64)
    logger.debug(response)


def usage(progname):
    print(f'''usage: {progname} file''')
    sys.exit(1)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        usage(sys.argv[0])

    path = sys.argv[1]

    endpoint_read, endpoint_write = usb_conf()
    upload(path, endpoint_read, endpoint_write)
    import ipdb; ipdb.set_trace()
