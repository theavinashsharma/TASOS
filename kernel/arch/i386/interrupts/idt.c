#include <stddef.h>
#include <stdint.h>

#include <tasos/idt.h>

#define CPU_EXCEPTION_COUNT 32

static struct idt_entry idt_entries[IDT_ENTRY_COUNT];
static struct idt_pointer idt_descriptor;

extern void idt_load(const struct idt_pointer* descriptor);

extern void (*isr_stub_table[CPU_EXCEPTION_COUNT])(void);

static void idt_clear(void)
{
    for (size_t index = 0; index < IDT_ENTRY_COUNT; index++) {
        idt_entries[index].offset_low = 0;
        idt_entries[index].selector = 0;
        idt_entries[index].reserved = 0;
        idt_entries[index].type_attributes = 0;
        idt_entries[index].offset_high = 0;
    }
}

void idt_set_gate(
    uint8_t vector,
    uintptr_t handler,
    uint16_t selector,
    uint8_t type_attributes
)
{
    idt_entries[vector].offset_low =
        (uint16_t)(handler & 0xFFFFU);

    idt_entries[vector].selector = selector;

    idt_entries[vector].reserved = 0;
    
    idt_entries[vector].type_attributes =
        type_attributes;

    idt_entries[vector].offset_high = 
        (uint16_t)((handler >> 16) & 0xFFFFU);
}

void idt_initialize(void)
{
    const uint8_t interrupt_gate_attributes =
        IDT_GATE_PRESENT |
        IDT_GATE_RING_0 |
        IDT_GATE_32_BIT_INTERRUPT;

    idt_clear();

    for (
        uint8_t vector = 0;
        vector < CPU_EXCEPTION_COUNT;
        vector++
    ) {
        idt_set_gate(
            vector,
            (uintptr_t)isr_stub_table[vector],
            IDT_KERNEL_CODE_SELECTOR,
            interrupt_gate_attributes
        );
    }

    idt_descriptor.limit =
        (uint16_t)(sizeof(idt_entries) - 1U);

    idt_descriptor.base =
        (uint32_t)(uintptr_t)&idt_entries[0];

    idt_load(&idt_descriptor);
}
