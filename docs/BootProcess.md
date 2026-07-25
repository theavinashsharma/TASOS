# TASOS Boot Process

## Initial Boot Strategy

TASOS initially uses GRUB and the Multiboot 1 specification.

GRUB is responsible for firmware-facing boot operations and loading the TASOS
kernel into memory. This lets early development focus on kernel architecture
rather than implementing a custom bootloader immediately.

## Initial Processor State

The first TASOS kernel entry point executes in 32-bit protected mode.

The initial boot entry performs the following operations:

1. Provides a valid Multiboot header
2. Reserves a 16 KiB kernel stack
3. Initializes the ESP stack pointer
4. Disables hardware interrupts
5. Calls the C function `kernel_main`
6. Halts safely if `kernel_main` returns

## Current Entry Flow

```text
Firmware
   |
   v
GRUB
   |
   v
Multiboot validation
   |
   v
_start
   |
   +--> Initialize stack
   |
   +--> Disable interrupts
   |
   v
kernel_main```

## Current Limitations

* The entry point is 32-bit
* No Interrupt Descriptor Table exists
* No paging is configured by TASOS
* No x86-64 long-mode transition exists
* No Multiboot information is currently passed to the C kernel

## Future Direction

A later milestone will build the required page tables and transition the
processor into x86-64 long mode before entering the main 64-bit kernel.
