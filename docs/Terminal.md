# TASOS Terminal Subsystem

## Purpose

The terminal subsystem provides text output for the TASOS kernel.

The initial implementation uses the VGA text buffer available on legacy
x86-compatible systems and virtual machines.

## Hardware Interface

The VGA text buffer begins at physical address:

```
0x000B8000
```

The standard text mode contains:
```
80 columns
25 rows
```
Each screen cell occupies two bytes:
1. ASCII character byte
2. Colour attribute byte

The colour attribute is arranged as:
```
Bits 0-3: foreground colour
Bits 4-7: background colour and blink information
```

## Responsibilities

The terminal subsystem is responsible for:
* Initializing the text display
* Clearing the screen
* Tracking row and column positions
* Writing individual characters
* Writing null-terminated strings
* Handling newline characters
* Scrolling when the final row is exceeded
* Managing foreground and background colours

## Public Interface

The public terminal API is declared in:
```
include/tasos/terminal.h
```
Hardware-specific implementation is stored in:
```
drivers/terminal/vga_terminal.c
```

## Architectural Boundary

Kernel startup code must use the public terminal interface.

It must not write directly to VGA memory.

This keeps hardware-specific behavior isolated from the rest of the kernel.

## Current Limitations
* VGA text mode only
* No Unicode support
* No hardware cursor update
* No formatted output
* No backspace or tab handling
* No framebuffer support

## Future Direction

Later terminal versions may provide:
* Integer and hexadecimal formatting
* Kernel log levels
* Hardware cursor movement
* ANSI-style control sequences
* Framebuffer-backed text rendering
* Virtual terminal sessions
