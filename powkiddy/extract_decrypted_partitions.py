#!/usr/bin/env python3
"""
Use this script on the decrypted partition ONLY
"""
import logging
import argparse
import struct
from collections import namedtuple
from typing import List, Tuple
import mmap
import pathlib


logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


OFFSET_ENTRIES = 0x200
BLOCK_SIZE = 0x200


Entry = namedtuple('Entry', [
    'label',
    'type',
    'offset',
    'size',
    'size_bis',
    'checksum',
])

def unpack_entry(offset: int, data: bytes) -> Entry:
    return Entry._make(struct.unpack_from("12sIIIII", data, offset=offset))


def unpack_module(entry: Entry, data: bytes) -> bytes:
    start, end = module_get_region(entry)
    return mem[start:end]


def module_get_region(entry: Entry) -> Tuple[int, int]:
    start = entry.offset * BLOCK_SIZE
    end = start + entry.size

    return start, end


def info_module(entry: Entry) -> None:
    start, end = module_get_region(entry)
    print("{}: 0x{:x}-0x{:x}".format(entry.label.decode("utf-8"), start, end))


def dump_module(entry: Entry, data: bytes, dir_dest: pathlib.Path) -> None:
    path = dir_dest / pathlib.Path(entry.label.decode("utf-8").replace(" ", "").replace("\x00", ""))

    logger.info("partition to path '%s', size of %d" % (path, entry.size))

    with open(path, "wb") as output:
        output.write(unpack_module(entry, data))


def get_modules(mem, offset=OFFSET_ENTRIES, size=0x20) -> List[Entry]:

    entries = []
    while True:

        entry = unpack_entry(offset, mem)

        if entry.label[0] == 0:  # TODO: why?
            break

        info_module(entry)

        entries.append(entry)
        offset += size

    return entries


def arguments():
    parser = argparse.ArgumentParser(description='extract modules from the decrypted partition')
    parser.add_argument('file', help='input file')
    parser.add_argument('--outdir', help='directory where extract the modules')

    return parser.parse_args()


if __name__ == '__main__':
    args = arguments()

    with open(args.file, "r+b") as f:
        mem = mmap.mmap(f.fileno(), 0)

        # TODO: check signature
        entries = get_modules(mem)

    if args.outdir:
        outdir = pathlib.Path(args.outdir)

        if not outdir.exists():
            raise Exception("Please create the directory '%s'" % outdir)

        for entry in entries:
            dump_module(entry, mem, outdir)
