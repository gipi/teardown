
# NOTE: use -fno-plt -fno-pic bc otherwise at linking time it tries to put
#       somewhere the relocations as globals.
#
# Use
#
#  $ mipsel-linux-gnu-objdump -b binary -mmips:isa32 -EL -D virus.bin
#
# to see the result
MIPSEL_PREFIX = mips-linux-gnu-
MIPSEL_OPTIONS = -march=4kec -EL -Wall
MIPSEL_LD_OPTIONS = -EL
%.bin: %.c startup.o
	$(MIPSEL_PREFIX)gcc $(MIPSEL_OPTIONS) -fno-plt -fno-pic -c $<
	$(MIPSEL_PREFIX)ld $(MIPSEL_LD_OPTIONS) -Tadfu.ld startup.o $*.o -o $*.elf
	$(MIPSEL_PREFIX)objcopy -O binary $*.elf $@

startup.o: startup.S
	$(MIPSEL_PREFIX)gcc $(MIPSEL_OPTIONS) -fno-plt -fno-pic -c $<

%.encrypted: %
	../meta/actions/adfuload encrypt $< > $@


clean:
	rm -f uart.bin.encrypted uart.bin startup.o
