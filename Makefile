CROSS    = aarch64-linux-gnu-
CC       = $(CROSS)gcc
CXX      = $(CROSS)g++
AS       = $(CROSS)gcc
LD       = $(CROSS)gcc
OBJCOPY  = $(CROSS)objcopy

CPU      = cortex-a72
TARGET   = rypos

# Directories
SRC_DIR   = src
INC_DIR   = include
BUILD_DIR = build
OBJ_DIR   = $(BUILD_DIR)/obj

# Include paths
INCLUDES = -I$(INC_DIR)/common -I$(INC_DIR)/kernel

# Flags
ASFLAGS  = -mcpu=$(CPU) -ffreestanding -g
CFLAGS   = -mcpu=$(CPU) -ffreestanding -std=gnu99 -O0 -Wall -Wextra $(INCLUDES) -g
CXXFLAGS = -mcpu=$(CPU) -ffreestanding -std=c++17 -O0 -Wall -Wextra \
           -fno-exceptions -fno-rtti -fno-threadsafe-statics $(INCLUDES) -g
LDFLAGS  = -T $(BUILD_DIR)/linker.ld -ffreestanding -O0 -nostdlib -lgcc

# QEMU
QEMU       = qemu-system-aarch64
MACHINE    = raspi4b
QEMU_FLAGS = -machine $(MACHINE) -nographic

# Sources
ASM_SRC = $(shell find $(SRC_DIR) -name '*.S')
C_SRC   = $(shell find $(SRC_DIR) -name '*.c')
CXX_SRC = $(shell find $(SRC_DIR) -name '*.cpp')

# Objects (mirror source tree under build/obj/)
ASM_OBJ = $(patsubst $(SRC_DIR)/%.S,$(OBJ_DIR)/%.o,$(ASM_SRC))
C_OBJ   = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRC))
CXX_OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CXX_SRC))
OBJ     = $(ASM_OBJ) $(C_OBJ) $(CXX_OBJ)

all: $(BUILD_DIR)/$(TARGET).img

$(BUILD_DIR)/$(TARGET).elf: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/$(TARGET).img: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BUILD_DIR)/$(TARGET).elf
	$(QEMU) $(QEMU_FLAGS) -kernel $<

debug: $(BUILD_DIR)/$(TARGET).elf
	$(QEMU) $(QEMU_FLAGS) -kernel $< -S -s

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).img

.PHONY: all run debug clean
