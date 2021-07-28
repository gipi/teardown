# generate a reduced ADEC skipping the first 0x400 bytes
# (that in practice are simply a jump and null bytes)
# this means we MUST load this at 0xb4060400
adec09_1.bin.decrypted.redux: adec09_1.bin.decrypted
	dd if=$^ of=$@ skip=2

adec09_1.bin.redux: adec09_1.bin.decrypted.redux
	../../meta/actions/adfuload encrypt $< > $@

clean:
	rm -f adec09_1.bin.redux adec09_1.bin.decrypted.redux
