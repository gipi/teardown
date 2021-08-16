# Makefile for building the project
#
# Large inspired by this answer about dependencies
#  <https://stackoverflow.com/a/30142139>.
# and this one about subdirectory rules
#  <https://stackoverflow.com/a/43227047>
#
ifeq ($(BOARD),)
$(error "please indicate a board with BOARD=")
endif

CONFIG = config/$(BOARD)

include $(CONFIG)

# change the shell to avoid weird syntax errors
# from command substitutions
SHELL := bash
SOURCE_DIR = src
BUILD_DIR = build_$(BOARD)

SOURCES_C-y             = $(SOURCE_DIR)/main.c
SOURCES_C-y            += $(SOURCE_DIR)/regs_io.c
SOURCES_C-$(USB)       += $(SOURCE_DIR)/usb.c
SOURCES_C-y            += $(SOURCE_DIR)/uart.c
SOURCES_C-y            += $(SOURCE_DIR)/utils.c
SOURCES_C-$(EXCEPTIONS) += $(SOURCE_DIR)/exceptions.c
SOURCES_C-y            += $(SOURCE_DIR)/log.c
SOURCES_C-y            += $(SOURCE_DIR)/assert.c
SOURCES_C-y            += $(SOURCE_DIR)/string.c
SOURCES_C-y            += $(SOURCE_DIR)/socs/$(shell echo $(SOC) | tr A-Z a-z)/hal.c
SOURCES_C-y            += $(SOURCE_DIR)/socs/$(shell echo $(SOC) | tr A-Z a-z)/brom.c

SOURCES_S-y             = $(SOURCE_DIR)/crt0.S
SOURCES_S-$(EXCEPTIONS)+= $(SOURCE_DIR)/eirq.S

OBJS_FROM_C = $(addprefix $(BUILD_DIR)/,$(SOURCES_C-y:%.c=%.o))
OBJS_FROM_S = $(addprefix $(BUILD_DIR)/,$(SOURCES_S-y:%.S=%.o))
OBJECTS = $(OBJS_FROM_C) $(OBJS_FROM_S)

DEPS := $(OBJS_FROM_C:.o=.d)



LD_SCRIPT = $(BUILD_DIR)/open-adec.ld
ADFULOAD  = ../../meta/actions/adfuload


# GENERAL COMPILER OPTIONS
MIPSEL_PREFIX = mips-linux-gnu-
MIPSEL_OPTIONS = -march=4kec -EL -Wall -fno-builtin -fno-plt -fno-pic -mno-abicalls -g -Iinclude/ -D$(SOC)=1 #-ffunction-sections -fdata-sections
MIPSEL_LD_OPTIONS = -EL -static -nostdlib #--gc-sections

# Avoid to:
# o  use make's built-in rules and variables
# o  print "Entering directory ...";
#MAKEFLAGS += --no-builtin-rules
#MAKEFLAGS += --no-builtin-variables
#MAKEFLAGS += --no-print-directory


# override this if you want visualized the command while executed
Q = @



$(info #################### Building for $(BOARD) ##############################)

all: $(BUILD_DIR) $(OPEN_ADEC) check-build

OPEN_ADEC = $(BUILD_DIR)/open-adec_$(BOARD).bin.encrypted

# FIXME: sometimes the encryption fails for the last dword :P
check-build: $(OPEN_ADEC) $(ADFULOAD)
	@echo "CHECK	$@"
	$(Q)diff -Nur <($(ADFULOAD) decrypt $< | xxd) <(xxd $(<:.encrypted=))

-include $(DEPS)

# TARGETS ###########################
$(BUILD_DIR):
	mkdir $@

$(ADFULOAD):
	@echo "ADFULOAD"
	$(Q)make -C $(dir $@)


$(BUILD_DIR)/open-adec_$(BOARD).elf: $(OBJECTS)
	@echo "ELF	$@"
	$(Q)$(MIPSEL_PREFIX)ld $(MIPSEL_LD_OPTIONS) -T$(LD_SCRIPT) $^ -o $@

$(BUILD_DIR)/open-adec_$(BOARD).elf: .EXTRA_PREREQS = $(LD_SCRIPT)


$(OBJS_FROM_S): $(BUILD_DIR)/%.o: %.S
	@echo "S -> O: $@"
	mkdir -p $(@D)
	$(Q)$(MIPSEL_PREFIX)gcc $(MIPSEL_OPTIONS) -DEXCEPTIONS=$(EXCEPTIONS) -c $< -o $@


$(OBJS_FROM_C): $(BUILD_DIR)/%.o: %.c
	@echo "C -> O: $@"
	mkdir -p $(@D)
	$(Q)$(MIPSEL_PREFIX)gcc $(MIPSEL_OPTIONS) -MMD -c $< -o $@

$(OBJS_FROM_C): $(CONFIG)
$(OBJS_FROM_S): $(CONFIG)


$(BUILD_DIR)/open-adec_$(BOARD).bin: $(BUILD_DIR)/open-adec_$(BOARD).elf
	@echo "BIN	$@"
	$(Q)$(MIPSEL_PREFIX)objcopy -O binary $< $@


# allow for the linker script to use #defines
$(BUILD_DIR)/%.ld: %.ld.in
	@echo "LDS	$@"
	$(Q)$(MIPSEL_PREFIX)cpp -x assembler-with-cpp -P \
		-D__ASSEMBLY__ \
		-DENTRY_ADDRESS=$(ENTRY_ADDRESS) \
		-DIRAM_SIZE=$(IRAM_SIZE)         \
		-o $@ $<

$(BUILD_DIR)/%.ld: $(CONFIG)


$(BUILD_DIR)/%.encrypted: $(BUILD_DIR)/% $(ADFULOAD)
	@echo "ENCRYPTION	$@"
	$(Q)$(ADFULOAD) encrypt $< > $@


upload: $(BUILD_DIR)/open-adec.bin.encrypted
	@echo "UPLOAD	$^"
	$(Q)PYTHONPATH=../../meta/actions/ python3 -m adfu.exe --device 10d6:10d6 --address $(ENTRY_ADDRESS) --binary $<

clean:
	@echo "CLEANING BUILD DIRECTORY"
	$(Q)rm -rf $(BUILD_DIR)/
