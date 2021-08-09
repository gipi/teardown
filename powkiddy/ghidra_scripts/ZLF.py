# Decompress the segment of the ZLF file following the __bcomprtab section
# this is a custom compressed variant of an ELF file used by the PowKiddy J6
#@author Gianluca Pacchiella
#@category
#@keybinding 
#@menupath 
#@toolbar 
import zlib
import struct
import jarray
from ghidra.program.model.address import AddressSet


# FileBytes provides access to the all the byte values
# (both original and modified) from an imported file.
file_bytes = currentProgram.getMemory().allFileBytes[0]

# get the section
bcomprtab = currentProgram.getMemory().getBlock('__bcomprtab')
# get its size
size = bcomprtab.getSize()
# extract the data inside
data = bytearray(bcomprtab.getData().readAllBytes())

# build the table with all the compressed part
table = [[entry for entry in struct.unpack('<IIII', data[index*0x10:(index*0x10) + 0x10])]
    for index in range(0, size >> 4)]

print [[hex(_) for _ in entry] for entry in table]

SEGMENT_PREFIX = 'segment_'
segment_name = [_.name for _ in currentProgram.getMemory().getBlocks() if _.name.startswith(SEGMENT_PREFIX)][0]
# get segment with executable memory
segment = currentProgram.getMemory().getBlock(segment_name)
# get the offset of the segment in the file
offset_file = segment.getSourceInfos()[0].getFileBytesOffset()
# create a buffer with all the data
data_segment = segment.getData().readAllBytes()

memory = currentProgram.getMemory()
memory_segment = getMemoryBlock(segment_name)

# first find the extension of the memory
# and in case create a new block
addr_max_available = memory_segment.getEnd().getOffset()
last_addr, _, last_length, _ = table[-1]
addr_final = last_addr + last_length

regions = []

def _get_bytes_from_file(off, size):
	v = jarray.zeros(size, "b")
	file_bytes.getOriginalBytes(off, v)

	return v

memory_block_index = 0
def _create_memory(startAddr, endAddr):
    global memory_block_index

    # add one byte othwerwise it's possible to have zero bytes range
    size = endAddr - startAddr + 1
    v = jarray.zeros(size, "b")

    createMemoryBlock(
        "DECOMPRESSED_%d" % memory_block_index,
        toAddr(startAddr),
        v,
        False)
    # you need to convert to initialized
    # currentProgram.getMemory().convertToInitialized(block_compressed, 0x00)

    memory_block_index += 1

def _check_and_create_memory(startAddr, endAddr):
    memory_range = AddressSet(toAddr(startAddr), toAddr(endAddr))
    # we look for the missing part of the range
    # in existing memory
    memory_missing = memory_range.subtract(memory)

    if not memory_missing.isEmpty():
        print "creating memory block for range %s" % memory_missing
        _create_memory(
            memory_missing.getMinAddress().getOffset(),
            memory_missing.getMaxAddress().getOffset())

for addr, offset, uncompressed_size, size in table:
    print("decompressing for address %08x" % addr)
    gzdata = _get_bytes_from_file(offset, size)
    regions.append((addr, zlib.decompress(gzdata)))

for addr, data in regions:
    print "writing back the uncompressed data for address %08x" % addr
    _check_and_create_memory(addr, addr + len(data))
    memory.setBytes(toAddr(addr), data)


