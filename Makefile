CC := gcc
AS := nasm
LD := ld

BUILD_DIR := build
ISO_DIR := iso

KERNEL := $(BUILD_DIR)/tasos.kernel
ISO_IMAGE := $(BUILD_DIR)/tasos.iso
MAP_FILE := $(BUILD_DIR)/tasos.map

CFLAGS := \
	-m32 \
	-std=gnu11 \
	-ffreestanding \
	-fno-pie \
	-fno-stack-protector \
	-Wall \
	-Wextra \
	-Werror \
	-O2 \
	-Iinclude

ASFLAGS := -f elf32

LDFLAGS := \
	-m elf_i386 \
	-T linker.ld \
	-Map $(MAP_FILE)

OBJECTS := \
	$(BUILD_DIR)/boot.o \
	$(BUILD_DIR)/kernel.o \
	$(BUILD_DIR)/vga_terminal.o \
	$(BUILD_DIR)/gdt.o \
	$(BUILD_DIR)/gdt_flush.o \
	$(BUILD_DIR)/idt.o \
	$(BUILD_DIR)/idt_load.o \
	$(BUILD_DIR)/isr.o \
	$(BUILD_DIR)/interrupt_handler.o \
	$(BUILD_DIR)/io.o \
	$(BUILD_DIR)/pic.o \
	$(BUILD_DIR)/irq.o \
	$(BUILD_DIR)/irq_handler.o \
	$(BUILD_DIR)/keyboard.o

.PHONY: all kernel iso run debug clean validate

all: iso

kernel: $(KERNEL)

iso: $(ISO_IMAGE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.o: boot/boot.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/kernel.o: kernel/kernel.c include/tasos/terminal.h include/tasos/gdt.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/vga_terminal.o: drivers/terminal/vga_terminal.c include/tasos/terminal.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/gdt.o: kernel/arch/i386/gdt/gdt.c include/tasos/gdt.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/gdt_flush.o: kernel/arch/i386/gdt/gdt_flush.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/idt.o: kernel/arch/i386/interrupts/idt.c include/tasos/idt.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/idt_load.o: kernel/arch/i386/interrupts/idt_load.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/isr.o: kernel/arch/i386/interrupts/isr.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/interrupt_handler.o: \
	kernel/arch/i386/interrupts/interrupt_handler.c \
	include/tasos/interrupt_frame.h \
	include/tasos/terminal.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/io.o: kernel/arch/i386/cpu/io.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/pic.o: drivers/interrupt_controller/pic.c \
	include/tasos/pic.h \
	include/tasos/io.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/irq.o: kernel/arch/i386/interrupts/irq.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/irq_handler.o: \
	kernel/arch/i386/interrupts/irq_handler.c \
	include/tasos/interrupt_frame.h \
	include/tasos/irq.h \
	include/tasos/pic.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/keyboard.o: drivers/input/keyboard/keyboard.c \
	include/tasos/interrupt_frame.h \
	include/tasos/io.h \
	include/tasos/irq.h \
	include/tasos/keyboard.h \
	include/tasos/terminal.h \
	include/tasos/pic.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


$(KERNEL): $(OBJECTS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

$(ISO_IMAGE): $(KERNEL) $(ISO_DIR)/boot/grub/grub.cfg
	cp $(KERNEL) $(ISO_DIR)/boot/tasos.kernel
	grub-mkrescue -o $@ $(ISO_DIR)

run: $(ISO_IMAGE)
	qemu-system-x86_64 \
		-cdrom $(ISO_IMAGE) \
		-m 256M \
		-no-reboot \
		-no-shutdown

debug: $(ISO_IMAGE)
	qemu-system-x86_64 \
		-cdrom $(ISO_IMAGE) \
		-m 256M \
		-no-reboot \
		-no-shutdown \
		-s \
		-S

validate: $(KERNEL) $(ISO_IMAGE)
	file $(KERNEL)
	file $(ISO_IMAGE)
	grub-file --is-x86-multiboot $(KERNEL)
	@echo "Multiboot validation passed."
	nm -u $(KERNEL)

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(ISO_DIR)/boot/tasos.kernel
	