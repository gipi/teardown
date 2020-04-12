#!/usr/bin/env python3
'''
Simple script to fuzz the ADFU mode of an AM7331 chip.
'''
import sys
import getopt
import serial
import time
import tqdm
import datetime
import logging
import random
import usb  # TODO: only for USBError, to improve interface
import adfu
from pathlib import Path


# logging.basicConfig()

stream = logging.StreamHandler()
formatter = logging.Formatter(' [+] %(message)s')

logger_app = logging.getLogger(f'{__file__}.app')
logger_app.setLevel(logging.DEBUG)
logger_app.addHandler(stream)
stream.setFormatter(formatter)


logger_results = logging.getLogger(f'{__file__}.results')
logger_results.setLevel(logging.INFO)
path_log_results = 'logs/{}_results.log'.format(
    str(datetime.datetime.today()).replace(' ', '_'),
)
handler_file_result = logging.FileHandler(path_log_results)
logger_results.addHandler(handler_file_result)


def usage(error_code):
    print(f'usage: {sys.argv[0]} --serial <serial device> --device <idVendor:idProduct> --firmware <directory with unpacked firmware>')
    sys.exit(error_code)


def parse_option():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "s:d:f:h", ["help", "firmware=", "device=", "serial=", ])
    except getopt.GetoptError as err:
        # print help information and exit:
        print(err)  # will print something like "option -value not recognized"
        usage(2)
    serial = None
    id_vendor, id_product = None, None
    path_fw = None
    verbose = False
    for o, value in opts:
        if o == "-v":
            verbose = True
        elif o in ("-h", "--help"):
            usage(1)
        elif o in ("-s", "--serial"):
            serial = value
        elif o in ("-d", "--device"):
            v, p = value.split(':')
            id_vendor = int(v, 16)
            id_product = int(p, 16)
        elif o in ("-f", '--firmware'):
            path_fw = value
        else:
            assert False, "unhandled option"

    return serial, (id_vendor, id_product), path_fw, verbose


def log_result():
    logger_results.info('miao')


def device_reset_to_adfu_mode(ser):
    ser.dtr = not ser.dtr


def wait_device(idVendor, idProduct):
    found = False
    while not found:
        try:
            # configure the endpoint for the bulk transfers
            found = adfu.cbw.usb_conf(*ids)
        # possible race condition with udev and permissions change
        except usb.core.USBError:
            pass
        time.sleep(1)

    return found


def get_random():
    return random.randrange(start=0, stop=0x10000)


def fuzz(e_read, e_write, seed):
    request_msg = None
    response_msg = None
    params = {
       'cmd': get_random()  & 0xff,
       'tag': get_random()  & 0xffffffff,
       'size': get_random() & 0xffffffff,
       'arg0': get_random() & 0xffffffff,
       'arg1': get_random() & 0xffffffff,
       'subCmd': get_random() & 0xffff,
       'subCmd2': get_random() & 0xffff,
       'flags': get_random() & 0xff,
    }
    request_msg = ', '.join(['{0}: {1:#0x}'.format(k, v) for k, v in params.items()])
    try:
        adfu.cbw.cbw_write_(
            e_write,
            **params,
        )

        response = bytes(e_read.read(0x100))
        response_msg = response.hex()
    except Exception as e:
        response_msg = str(e)

    logger_results.info(f'{request_msg} {response_msg}')


def upload_fw(r, w, path_fw):
    path_adec = Path(path_fw) / 'ADECadfus'
    path_adfu = Path(path_fw) / 'ADFUadfus'
    adfu.cbw.ADECadfus(path_adec, r, w)
    adfu.cbw.ADFUadfus(path_adfu, r, w)


if __name__ == '__main__':
    path_serial, ids, path_fw, verbose = parse_option()

    if not path_serial or not ids[0] or not ids[1] or not path_fw:
        usage(3)

    logger_app.info(f'logging results into {path_log_results}')
    logger_app.info(f'using serial {path_serial}')
    logger_app.info(f'fuzzing device {ids[0]:x}:{ids[1]:x}')
    logger_app.info(f'using unpacked firmware from {path_fw}')

    s = serial.Serial(path_serial, 115200)
    # TODO: check is not already opened
    logger_app.info(f'opened serial successfully')

    logger_app.info('starting to fuzz')

    time.sleep(4)  # allow to disconnect previous device

    from usb.util import dispose_resources

    for _ in tqdm.trange(0x10000):
        device_reset_to_adfu_mode(s)
        time.sleep(1)  # race condition between reset and wait
        dev, e_read, e_write = wait_device(*ids)
        upload_fw(e_read, e_write, path_fw)
        fuzz(e_read, e_write, _)
        dispose_resources(dev)  # free open files
