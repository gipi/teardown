import logging
import argparse
import functools
from .cbw import usb_conf, ADECadfus, _cbw_write, cbw_read_response


logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


def argparse_vendor_product(value):
    vendor, product = tuple(value.split(":"))

    return int(vendor, 16), int(product, 16)


def parse_args():
    args = argparse.ArgumentParser(description='upload and run some code')

    args.add_argument(
        '--device',
        type=argparse_vendor_product,
        required=True,
        help="vendor:product of the device you want to interact with")
    args.add_argument('--binary', required=True)
    args.add_argument('--address', type=functools.partial(int, base=0), default=0xb4060000)

    return args.parse_args()


if __name__ == '__main__':
    args = parse_args()

    dev, endpoint_read, endpoint_write = usb_conf(*args.device)

    if not dev:
        raise Exception("device not found")

    _cbw_write(endpoint_write, 0xcc, 0x00, 0x00, 0x00, 0x00)

    endpoint_read.read(0x12)

    cbw_read_response(endpoint_read)

    logger.info(f"uploading binary '{args.binary}' and executing it")
    # try to use the standard code from Actions
    ADECadfus(args.binary, endpoint_read, endpoint_write, address=args.address)

    import time

    while True:
        _cbw_write(endpoint_write, 0xab, 0x00, 0xcafebabe, 0xdeadbeef, 0xbadc0de)
        cbw_read_response(endpoint_read)
        time.sleep(1)
