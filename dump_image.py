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
import sys
#from pwnlib.tubes.remote import remote
from pwn import log as logger, context

from telnetlib import Telnet # telnet has its own protocol wo this we lost console data


START_ADDRESS  = 0x90000000
BLOCK_SIZE     = 0x1000
FMT_DUMP_IMAGE = 'dump_image DSL-302T/cfi.%d.img 0x%x 0x%x\n'
FMT_VRFY_IMAGE = 'verify_image DSL-302T/cfi.%d.img 0x%x\n'

if __name__ == '__main__':
    n = 0x00200000/0x1000
    logger.info('dumping %d block' % n)

    #r = remote('localhost', 4444)
    telnet = Telnet('localhost', 4444)

    msg = telnet.read_until('\n\r> ')

    # first of all check that makes sense
    #r.sendline('targets')

    telnet.write('targets\n')

    msg = telnet.read_until('\n\r> ')

    logger.debug(msg)

    #sys.exit(0)

    #r.clean_and_log()
    #r.clean_and_log()

    for idx in xrange(n):
        address = START_ADDRESS + idx*BLOCK_SIZE
        cmd = FMT_DUMP_IMAGE  % (idx, address, BLOCK_SIZE)

        logger.info(cmd)
        telnet.write(cmd)
        output = telnet.read_until('\n\r> ', timeout=1000)

        logger.info(output)

        cmd = FMT_VRFY_IMAGE % (idx, address)
        logger.info(cmd)
        telnet.write(cmd)

        output = telnet.read_until('\n\r> ', timeout=1000)

        logger.info(output)
