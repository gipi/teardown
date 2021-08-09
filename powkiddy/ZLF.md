# ZLF

Custom compressed ELF file used by the PowKiddy operating system.

The section named ``__bcomprtab`` contains a certain number of entries composed
of four elements

 - address where the decompressed data is going
 - offset in the file where the compressed data start
 - length of the uncompressed data
 - length of the compressed data

The ``ghidra``'s loader fails to load all the needed segments since the program
header indicates offset that are not present in the file itself.

It's possible to use the [LZF ghidra's script](ghidra_scripts/ZLF.py) to
recreate the "original" ELF.
