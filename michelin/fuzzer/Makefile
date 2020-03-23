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
	rm -f ${BIN}.elf ${BIN}.hex ${OBJS}
