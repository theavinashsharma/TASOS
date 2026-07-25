.PHONY: help verify clean

help:
	@echo "TASOS build system"
	@echo ""
	@echo "Available targets:"
	@echo "  make verify  - Verify required development tools"
	@echo "  make clean   - Remove generated build files"

verify:
	@command -v gcc >/dev/null || { echo "Missing gcc"; exit 1; }
	@command -v nasm >/dev/null || { echo "Missing nasm"; exit 1; }
	@command -v make >/dev/null || { echo "Missing make"; exit 1; }
	@command -v gdb >/dev/null || { echo "Missing gdb"; exit 1; }
	@command -v grub-mkrescue >/dev/null || { echo "Missing grub-mkrescue"; exit 1; }
	@command -v xorriso >/dev/null || { echo "Missing xorriso"; exit 1; }
	@command -v qemu-system-x86_64 >/dev/null || { echo "Missing qemu-system-x86_64"; exit 1; }
	@echo "TASOS development environment verified."

clean:
	@find build -mindepth 1 ! -name .gitkeep -delete
	@find iso -mindepth 1 ! -name .gitkeep -delete
	@echo "Build directories cleaned."
