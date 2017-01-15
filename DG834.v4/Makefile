# from raw to ELF
# https://stackoverflow.com/questions/38951492/linkers-emulation-vs-output-format
#

OBJCOPY_OUTPUT_TARGET = elf32-tradbigmips
OBJCOPY_BINARY_ARCH   = mips

LD_EMULATION          = elf32btsmip
LD_LINKER_SCRIPT      = netgear.ld
%_from_raw.o: %.raw
	mipsel-linux-gnu-objcopy --input-target binary \
		--output-target $(OBJCOPY_OUTPUT_TARGET) \
		--binary-architecture $(OBJCOPY_BINARY_ARCH) $< $@

%_from_raw.elf: %_from_raw.o
	mipsel-linux-gnu-ld \
		-m $(LD_EMULATION) \
		-T $(LD_LINKER_SCRIPT) $< -o $@
