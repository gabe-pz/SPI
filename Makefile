
TARGET   = firmware
SRCS = $(notdir $(wildcard src/*.c sys/*.c include/*.c))
LDSCRIPT = sys/linker_script.ld
MCU_FLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

# ---- Toolchain ----
PREFIX   = arm-none-eabi-
CC       = $(PREFIX)gcc
LD       = $(PREFIX)gcc
OBJCOPY  = $(PREFIX)objcopy
SIZE     = $(PREFIX)size

BUILD_DIR = build

CFLAGS   = $(MCU_FLAGS) -Wall -O0 -g -std=c11 -ffreestanding
CFLAGS  += -Iinclude

LDFLAGS  = $(MCU_FLAGS) -T$(LDSCRIPT) -nostdlib -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.c=.o)))

vpath %.c src sys include

.PHONY: all clean flash

all: $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILD_DIR)

flash: all
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
	  -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"