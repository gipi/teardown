CFLAGS=-Wall -g


TARGETS  = tcctool
TARGETS += tccvirus.bin
TARGETS += tcc-emu

all: $(TARGETS)

tccvirus.bin: tccvirus.s
	arm-none-eabi-as -Wall $^ -o tccvirus.o
	arm-none-eabi-ld -Ttccvirus.ld tccvirus.o -o tccvirus
	arm-none-eabi-objcopy -O binary tccvirus tccvirus.bin

tcc-emu: CC=arm-linux-gnueabi-gcc
tcctool: LDLIBS+=-lusb

clean:
	rm -vfr $(TARGETS)

