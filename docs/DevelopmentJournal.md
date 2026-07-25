# TASOS Development Journal

## Day 1: Development Environment

### Completed

- Enabled and verified WSL2
- Installed Ubuntu 24.04 LTS
- Installed GCC, NASM, Make, GDB and Git
- Installed GRUB, xorriso and mtools
- Installed and tested QEMU
- Compiled a freestanding x86_64 C object
- Assembled an x86_64 NASM object
- Created the initial NovaOS project structure

### Environment

- Host: Windows 11
- Linux environment: Ubuntu 24.04 LTS through WSL2
- Architecture: x86_64
- Compiler: GCC 13.3.0
- Assembler: NASM 2.16.01
- Build tool: GNU Make 4.3
- Debugger: GDB 15.1
- Emulator: QEMU 8.2.2
- Bootloader tooling: GRUB 2.12

### Problems Encountered

NASM was initially unavailable because the local APT package index had not yet been refreshed.

Running `sudo apt update` resolved the issue.

### Learned

- WSL2 can provide a complete OS-development toolchain on Windows.
- Freestanding C can be compiled without linking against a hosted runtime.
- NASM can produce x86_64 ELF object files.
- QEMU can run successfully through WSLg.
- GRUB and xorriso will be used to produce the first bootable ISO.

### Next Goal

Build the smallest possible bootable TASOS kernel.
