# Makefile for building the project
#
# Large inspired by this answer about dependencies
#  <https://stackoverflow.com/a/30142139/1935366>.
#

# change the shell to avoid weird syntax errors
# from command substitutions
SHELL := bash
SOURCE_DIR = src
BUILD_DIR = build

SOURCES_C  = main.c
SOURCES_C += regs_io.c
SOURCES_C += usb.c
SOURCES_C += uart.c
SOURCES_C += hal.c
SOURCES_C += utils.c
SOURCES_C += exceptions.c
SOURCES_C += log.c
SOURCES_C += brom.c
SOURCES_C += assert.c
SOURCES_C += string.c

SOURCES_S  = crt0.S
SOURCES_S += eirq.S

OBJS_FROM_C = $(SOURCES_C:%.c=$(BUILD_DIR)/%.o)
OBJS_FROM_S = $(SOURCES_S:%.S=$(BUILD_DIR)/%.o)

DEPS := $(OBJS_FROM_C:.o=.d)


LD_SCRIPT = $(BUILD_DIR)/open-adec.ld
ADFULOAD  = ../../meta/actions/adfuload

ENTRY_ADDRESS = 0xb4060000

# GENERAL COMPILER OPTIONS
MIPSEL_PREFIX = mips-linux-gnu-
MIPSEL_OPTIONS = -march=4kec -EL -Wall -fno-builtin -fno-plt -fno-pic -mno-abicalls -g -Iinclude/
MIPSEL_LD_OPTIONS = -EL

# Avoid to:
# o  use make's built-in rules and variables
# o  print "Entering directory ...";
#MAKEFLAGS += --no-builtin-rules
#MAKEFLAGS += --no-builtin-variables
#MAKEFLAGS += --no-print-directory

# override this if you want visualized the command while executed
Q = @

all: $(BUILD_DIR) $(BUILD_DIR)/open-adec.bin.encrypted check-build

# FIXME: sometimes the encryption fails for the last dword :P
check-build: $(BUILD_DIR)/open-adec.bin.encrypted $(ADFULOAD)
	@echo "CHECK	$@"
	$(Q)diff -Nur <($(ADFULOAD) decrypt $< | xxd) <(xxd $(<:.encrypted=))

-include $(DEPS)

# TARGETS ###########################
$(BUILD_DIR):
	mkdir $@

$(ADFULOAD):
	@echo "ADFULOAD"
	$(Q)make -C $(dir $@)

$(BUILD_DIR)/open-adec.elf: $(OBJS_FROM_C) $(OBJS_FROM_S)
	@echo "ELF	$@"
	$(Q)$(MIPSEL_PREFIX)ld $(MIPSEL_LD_OPTIONS) -T$(LD_SCRIPT) $^ -o $@

$(BUILD_DIR)/open-adec.elf: .EXTRA_PREREQS = $(LD_SCRIPT)

$(BUILD_DIR)/open-adec.bin: $(BUILD_DIR)/open-adec.elf
	@echo "BIN	$@"
	$(Q)$(MIPSEL_PREFIX)objcopy -O binary $< $@

# allow for the linker script to use #defines
$(BUILD_DIR)/%.ld: %.ld.in
	@echo "LDS	$@"
	$(Q)$(MIPSEL_PREFIX)cpp -x assembler-with-cpp -P -D__ASSEMBLY__  -o $@ $<

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.S
	@echo "S -> o: $@"
	$(Q)$(MIPSEL_PREFIX)gcc $(MIPSEL_OPTIONS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@echo "c -> o: $@"
	$(Q)$(MIPSEL_PREFIX)gcc $(MIPSEL_OPTIONS) -MMD -c $< -o $@

$(BUILD_DIR)/%.encrypted: $(BUILD_DIR)/% $(ADFULOAD)
	@echo "ENCRYPTION	$@"
	$(Q)$(ADFULOAD) encrypt $< > $@

upload: $(BUILD_DIR)/open-adec.bin.encrypted
	@echo "UPLOAD	$^"
	$(Q)PYTHONPATH=../../meta/actions/ python3 -m adfu.exe --device 10d6:10d6 --address $(ENTRY_ADDRESS) --binary $<

clean:
	@echo "CLEANING BUILD DIRECTORY"
	$(Q)rm -rf $(BUILD_DIR)/
