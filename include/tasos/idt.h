#ifndef TASOS_IDT_H
#define TASOS_IDT_H

#include <stddef.h>
#include <stdint.h>

#define IDT_ENTRY_COUNT 256

#define IDT_GATE_PRESENT 0x80
#define IDT_GATE_RING_0 0x00
#define IDT_GATE_RING_3 0x60
#define IDT_GATE_32_BIT_INTERRUPT 0x0E
#define IDT_GATE_32_BIT_TRAP 0x0F

#define IDT_KERNEL_CODE_SELECTOR 0x08

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t reserved;
    uint8_t type_attributes;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_pointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void idt_initialize(void);

void idt_set_gate(
    uint8_t vector,
    uintptr_t handler,
    uint16_t selector,
    uint8_t type_attributes
);

#endif
