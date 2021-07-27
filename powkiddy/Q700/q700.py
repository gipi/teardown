#!/usr/bin/env python3
"""
Script to interact with the ADFU mode of the Q700.
"""
import os
import sys
import argparse
import logging
from hexdump import hexdump


logging.basicConfig()
logger = logging.getLogger(__name__)  # application level logger
logger.setLevel(logging.INFO)


# import our module
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "..", "meta", "actions"))
import adfu

# identifier for the Powkiddy J6
VENDOR_ID = 0x10d6
PRODUCT_ID = 0x10d6


def q700_read(ep_r, ep_w, address, size):
    """From the adec binary is possible to see that exists a command that is
    able to dump memory from arbitrary addresses."""
    adfu.cbw.cbw_write_(
        ep_w,
        cmd=0x10, # must not be 0xb0
        tag=0x00, # doesn't matter
        size=0x1f,
        arg0=0x13 | 0x80, # the MSB is the direction
        arg1=size,
        subCmd=address & 0xffff,
        subCmd2=(address & 0xffff0000) >> 16,
    )
    response = ep_r.read(size)
    logger.debug('response:\n' + hexdump(response, result='return'))
    adfu.cbw.cbw_read_response(ep_r, tag=None)

    return response


def argparse_auto_int(value):
    return int(value, 0)


def parse_args():
    parser = argparse.ArgumentParser(description='interact with the ADFU mode of the Q700')
    parser.add_argument('--adec', help='adec09_1.bin file', required=True)

    subparsers = parser.add_subparsers(dest='cmd', help='raw or hex dump')
    subparsers.required = True

    parser_dump = subparsers.add_parser('dump')
    parser_hexdump = subparsers.add_parser('hexdump')

    parser.add_argument('address', type=argparse_auto_int, help='starting address')
    parser.add_argument('size', type=argparse_auto_int, help='number of bytes to dump')

    return parser.parse_args()


if __name__ == '__main__':
    args = parse_args()

    dev, endpoint_read, endpoint_write = adfu.cbw.usb_conf(VENDOR_ID, PRODUCT_ID)

    if not dev:
        raise Exception("device not found")

    adfu.cbw.ADECadfus(args.adec, endpoint_read, endpoint_write, address=0xb4060000)

    logger.info(f"dumping from {args.address:x} for {args.size} bytes, interval {args.address:x}-{args.address + args.size:x}")
    response = q700_read(endpoint_read, endpoint_write, args.address, args.size)

    if args.cmd == "hexdump":
        response = hexdump(response, result='return')
        print(response)
    else:
        sys.stdout.buffer.write(response)
