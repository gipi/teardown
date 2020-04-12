# Note: depending on the board's bootloader you
#       need a different AVRDUDE_PROTOCOL, so far
#       I encountered the following
#
#       arduino nano atmega32u4 -> avr109 57600
#       wemos mega              -> wiring 115200
BIN=fuzz
OBJS=fuzz.o

MCU=atmega328p
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-Wall -g -Os -DF_CPU=16000000UL -mmcu=$(MCU)
AVRDUDE_PROTOCOL=arduino
AVRDUDE_PORT=/dev/ttyUSB1
AVRDUDE_BAUDRATE=115200

# PORT=/dev/ttyACM0

${BIN}.hex: ${BIN}.elf
	${OBJCOPY} -j .text -j .data -O ihex $(BIN).elf $(BIN).hex

${BIN}.elf: ${OBJS}
	${CC} $(CFLAGS) -o $@ $^

flash: ${BIN}.hex
	avrdude -v -c $(AVRDUDE_PROTOCOL) -p $(MCU) -P $(AVRDUDE_PORT) -b $(AVRDUDE_BAUDRATE) -U flash:w:$<:i

clean:
	rm -f ${BIN}.elf ${BIN}.hex ${OBJS} virus.o virus.elf virus.bin

# NOTE: use -fno-plt -fno-pic bc otherwise at linking time it tries to put
#       somewhere the relocations as globals.
#
# Use
#
#  $ mipsel-linux-gnu-objdump -b binary -mmips:isa32 -EL -D virus.bin
#
# to see the result
MIPSEL_PREFIX = mipsel-linux-gnu-
virus.bin: virus.c
	$(MIPSEL_PREFIX)gcc -fno-plt -fno-pic -c $<
	$(MIPSEL_PREFIX)ld -Tvirus.ld virus.o -o virus.elf
	$(MIPSEL_PREFIX)objcopy -O binary virus.elf $@
