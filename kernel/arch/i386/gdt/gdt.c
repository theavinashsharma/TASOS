#include <stddef.h>
#include <stdint.h>

#include <tasos/gdt.h>

#define GDT_ENTRY_COUNT 3

#define GDT_ACCESS_PRESENT 0x80
#define GDT_ACCESS_RING_0 0x00
#define GDT_ACCESS_DESCRIPTOR 0x10
#define GDT_ACCESS_EXECUTABLE 0x08
#define GDT_ACCESS_READ_WRITE 0x02

#define GDT_FLAG_GRANULARITY_4K 0x80
#define GDT_FLAG_32_BIT 0x40

#define GDT_KERNEL_CODE_SELECTOR 0x08
#define GDT_KERNEL_DATA_SELECTOR 0x10

static struct gdt_entry gdt_entries[GDT_ENTRY_COUNT];
static struct gdt_pointer gdt_descriptor;

extern void gdt_flush(
    const struct gdt_pointer* descriptor,
    uint16_t code_selector,
    uint16_t data_selector
);

static void gdt_set_entry(
    size_t index,
    uint32_t base,
    uint32_t limit,
    uint8_t access,
    uint8_t flags
)
{
    gdt_entries[index].base_low =
        (uint16_t)(base & 0xFFFF);

    gdt_entries[index].base_middle = 
        (uint8_t)((base >> 16) & 0xFF);

    gdt_entries[index].base_high = 
        (uint8_t)((base >> 24) & 0xFF);

    gdt_entries[index].limit_low =
        (uint16_t)(limit & 0xFFFF);

    gdt_entries[index].granularity =
        (uint8_t)((limit >> 16) & 0x0F);

    gdt_entries[index].granularity |=
        (uint8_t)(flags & 0xF0);

    gdt_entries[index].access = access;
}

void gdt_initialize(void)
{
    gdt_descriptor.limit =
        (uint16_t)(sizeof(gdt_entries) - 1);

    gdt_descriptor.base =
        (uint32_t)(uintptr_t)&gdt_entries[0];

    gdt_set_entry(
        0,
        0,
        0,
        0,
        0
    );

    gdt_set_entry(
        1,
        0,
        0x000FFFFF,
        GDT_ACCESS_PRESENT |
            GDT_ACCESS_RING_0 |
            GDT_ACCESS_DESCRIPTOR |
            GDT_ACCESS_EXECUTABLE |
            GDT_ACCESS_READ_WRITE,
        GDT_FLAG_GRANULARITY_4K |
            GDT_FLAG_32_BIT
    );

    gdt_set_entry(
        2,
        0,
        0x000FFFFF,
        GDT_ACCESS_PRESENT |
            GDT_ACCESS_RING_0 |
            GDT_ACCESS_DESCRIPTOR |
            GDT_ACCESS_READ_WRITE,
        GDT_FLAG_GRANULARITY_4K |
            GDT_FLAG_32_BIT
    );

    gdt_flush(
        &gdt_descriptor,
        GDT_KERNEL_CODE_SELECTOR,
        GDT_KERNEL_DATA_SELECTOR
    );
}
