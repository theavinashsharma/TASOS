# TASOS Kernel Linking

## Purpose

The linker combines the assembly boot object and the C kernel object into one
executable kernel image.

## Entry Point

The kernel entry point is `_start`, which is defined in `boot/boot.asm`.

## Load Address

The kernel is linked to begin at physical address 1 MiB:

```
0x00100000
```
This avoids low-memory regions traditionally used by firmware and legacy x86
structures.

## Section Order

The initial kernel uses the following output section order:

1. Multiboot header
2. Executable code
3. Read-only data
4. Initialized data
5. Uninitialized data

The Multiboot header is placed first so GRUB can locate it near the beginning
of the kernel file.

## Alignment

Kernel sections are aligned to 4 KiB boundaries.

This is not strictly required for the first boot, but it prepares the layout
for future paging support.

## Current Linker

The initial kernel is linked using GNU ld with the elf_i386 output format.
