#!/usr/bin/env python3
"""
From a flash dump containing the "ActBrm" signature at offset 0x1f4
tries to extract the partitions as indicated at offset 0x2000.
"""
import logging
import sys
import struct
from collections import namedtuple
import mmap


logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


OFFSET_ACTBRM = 0x1f4
SIGNATURE_ACTBRM = b"ActBrm"  # FIXME: add complete signature

OFFSET_MBREC = 0x2000
BLOCK_SIZE = 0x200

Entry = namedtuple('Entry', [
    'label',
    'type',
    'unknown',
    'offset',
    'size',
    'checksum',
])


def unpack_entry(offset: int, data: bytes) -> Entry:
    return Entry._make(struct.unpack_from("12sIIIII", data, offset=offset))


def unpack_partition(entry: Entry, data: bytes) -> bytes:
    start = entry.offset * BLOCK_SIZE
    end = start + entry.size
    return mem[start:end]

def info_partition(entry: Entry) -> None:
    start = entry.offset * BLOCK_SIZE
    end = start + entry.size
    print("{}: 0x{:x}-0x{:x}".format(entry.label.decode("utf-8"), start, end))


def dump_partition(entry: Entry, data: bytes) -> None:
    path = entry.label.decode("utf-8").replace("\x00", "")

    logger.info("partition to path '%s', size of %d" % (path, entry.size))

    with open(path, "wb") as output:
        output.write(unpack_partition(entry, data))


if __name__ == '__main__':
    path = sys.argv[1]
    dump = False  # TODO: use argparse to make a little more a swiss knife

    logger.info("opening '%s'" % path)

    with open(path, "r+b") as f:
        mem = mmap.mmap(f.fileno(), 0)  # map the complete file

        if not mem[OFFSET_ACTBRM:OFFSET_ACTBRM + len(SIGNATURE_ACTBRM)] == SIGNATURE_ACTBRM:
            raise Exception("The signature is not present") 

        logger.debug("found %s signature" % SIGNATURE_ACTBRM)

        entries = []
        offset = OFFSET_MBREC
        while True:

            entry = unpack_entry(offset, mem)

            # breakpoint()

            if entry.label[0] == 0:  # TODO: why?
                break

            logger.debug(repr(entry))

            entries.append(entry)
            offset += 0x20

        for entry in entries:
            if dump:
                dump_partition(entry, mem)
            else:
                info_partition(entry)
