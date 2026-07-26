# TASOS Interrupt Architecture

## Purpose

The interrupt subsystem allows TASOS to respond to processor exceptions,
software interrupts, and hardware interrupt requests.

The first implementation targets 32-bit x86 protected mode.

## Interrupt Descriptor Table

The Interrupt Descriptor Table contains 256 entries.

Each entry associates an interrupt vector with:

- A handler address
- A code segment selector
- A gate type
- A privilege level
- A present flag

The processor locates the table through the IDTR register.

## Vector Ranges

The initial vector layout is:

```text
0-31    CPU exceptions
32-47   Hardware IRQs after PIC remapping
48-255  Available for software and future platform interrupts
```

## IDT Entry Layout

Each 32-bit IDT entry occupies eight bytes:
```
Offset low
Code segment selector
Reserved byte
Type and attribute byte
Offset high
```
The structures are packed because the processor requires an exact binary
layout.

## Initial Gate Type

TASOS initially uses 32-bit interrupt gates with the following attribute:
```
0x8E
```
This represents:
* Present
* Ring 0
* 32-bit interrupt gate

## Exception Handling Plan

Each CPU exception will enter an assembly stub.

The stub will normalize the stack layout and transfer control to a common C
handler.

The C handler will display diagnostic information and safely halt the kernel.

## Current Limitations
* Exception gates are not yet installed
* No common interrupt frame exists yet
* Hardware IRQs are not enabled
* The PIC has not been remapped
* Interrupts remain disabled
* No user-mode interrupt support exists

## Future Direction

Later stages will add:
* CPU exception stubs
* Register-state diagnostics
* PIC remapping
* Keyboard IRQ handling
* PIT timer interrupts
* Page-fault address reporting
* System-call gates
