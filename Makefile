CC := clang-18

BIN := kexp.bin

SRC_DIR := src
BUILD_DIR := build

TARGET := -target x86_64-linux-gnu --sysroot=/usr/x86_64-linux-gnu

CFLAGS := $(TARGET) -O3 -fPIE -ffreestanding \
	    -fno-omit-frame-pointer -Iinclude -Wall -Wextra -Werror \
		-Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-uninitialized -Wno-int-conversion

LDFLAGS := $(TARGET) -fuse-ld=lld -nostdlib -nostartfiles -static -Wl,--build-id=none

OBJS := \
  $(BUILD_DIR)/api.o \
  $(BUILD_DIR)/main.o \
  $(BUILD_DIR)/iommu.o \
  $(BUILD_DIR)/logger.o \
  $(BUILD_DIR)/loader.o \
  $(BUILD_DIR)/kernel.o \
  $(BUILD_DIR)/syscalls.o \

all: $(BUILD_DIR)/$(BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/syscalls.S: $(SRC_DIR)/syscalls.S $(SRC_DIR)/syscalls.defs
	cp $< $@

	awk -F, '\
	BEGIN { \
    	print "\n"; \
	} \
	{ \
    	print ".global " $$1 "_stub"; \
    	print $$1 "_stub:"; \
    	print "    mov rax, " $$2; \
    	print "    jmp syscall"; \
    	print ""; \
	}' $(word 2,$^) >> $@

$(BUILD_DIR)/syscalls.ld: $(SRC_DIR)/syscalls.defs
	awk -F, '{print "PROVIDE(" $$1 " = " $$1 "_stub);"}' $< > $@

$(BUILD_DIR)/syscalls.o: $(BUILD_DIR)/syscalls.S
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	
OBJCOPY := llvm-objcopy

$(BUILD_DIR)/$(BIN): $(OBJS) script.ld $(BUILD_DIR)/syscalls.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(BUILD_DIR)/kexp.elf -Tscript.ld -T$(BUILD_DIR)/syscalls.ld
	$(OBJCOPY) -O binary $(BUILD_DIR)/kexp.elf $@

clean:
	rm -rf $(BUILD_DIR)