#!/usr/bin/env python2
'''
Since JTAG is a slow af, this script dumps a block at times and verifies it.

Pratically it does dump_image and verify_image in loop.

    > flash banks  
    #0 : 0 (cfi) at 0x90000000, size 0x00200000, buswidth 2, chipwidth 2

# TODO

 - save log to file and use progress bar
 - add checks (files exist and are not empty)
 - add options (size, start address and block count)
 - build image at the end of the dump process
 - add protocol (this is JTAG, add serial support [dump from /dev/mtd])
'''
import os
import sys
import argparse
#from pwnlib.tubes.remote import remote
from pwn import log as logger, context

from telnetlib import Telnet # telnet has its own protocol wo this we lost console data


START_ADDRESS  = 0x90000000
BLOCK_SIZE     = 0x1000
FMT_DUMP_IMAGE = 'dump_image DSL-302T/cfi.%d.img 0x%x 0x%x\n'
FMT_VRFY_IMAGE = 'verify_image DSL-302T/cfi.%d.img 0x%x\n'


# https://stackoverflow.com/questions/25513043/python-argparse-fails-to-parse-hex-formatting-to-int-type
def auto_int(x):
    return int(x, 0)

def options():
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', '--dir', help='output directory', default='.')
    parser.add_argument('-b', '--base', help='base name', default='dump')
    parser.add_argument('-s', '--start', type=auto_int, help='start block number', default=0)
    parser.add_argument('-e', '--end',   type=auto_int, help='end block number')
    parser.add_argument('address', type=auto_int, help='address of the flash')
    parser.add_argument('size', type=auto_int, help='total size to dump')
    parser.add_argument('block', type=auto_int, help='size of individual block to dump', nargs='?', default=0x1000)

    return parser.parse_args()


if __name__ == '__main__':
    args = options()

    print args

    n = args.size/args.block
    logger.info('dumping %d block' % n)

    fmt_path = os.path.join(args.dir, '%s.%%d.img' % args.base)

    logger.info('format string for path: \'%s\'' % fmt_path)

    telnet = Telnet('localhost', 4444)

    msg = telnet.read_until('\n\r> ')

    telnet.write('targets\n')

    msg = telnet.read_until('\n\r> ')

    logger.debug(msg)

    for idx in xrange(args.start, args.end + 1 if args.end else n):
        address = START_ADDRESS + idx*BLOCK_SIZE
        filepath = fmt_path % idx
        cmd = 'dump_image %s 0x%x 0x%x\n'  % (filepath, address, args.block)

        logger.info(cmd)
        telnet.write(cmd)
        output = telnet.read_until('\n\r> ', timeout=1000)

        logger.info(output)

        # check if the file isn't empty
        if os.stat(filepath).st_size == 0:
            raise ValueError('dump of zero size') 

        cmd = 'verify_image %s 0x%x\n' % (filepath, address)
        logger.info(cmd)
        telnet.write(cmd)

        output = telnet.read_until('\n\r> ', timeout=1000)

        logger.info(output)
