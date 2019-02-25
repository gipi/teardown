CFLAGS=-Wall
LDLIBS=-lusb

TARGETS  = tcctool
TARGETS += tccvirus.bin

all: $(TARGETS)

tccvirus.bin: tccvirus.s
	arm-none-eabi-as -Wall $^ -o tccvirus.o
	arm-none-eabi-ld -Ttccvirus.ld tccvirus.o -o tccvirus
	arm-none-eabi-objcopy -O binary tccvirus tccvirus.bin

clean:
	rm -vfr $(TARGETS)

