#include <tasos/gdt.h>
#include <tasos/idt.h>
#include <tasos/terminal.h>
#include <tasos/irq.h>
#include <tasos/pic.h>
#include <tasos/keyboard.h>

void kernel_main(void)
{
    terminal_initialize();

    terminal_set_color(
        TERMINAL_COLOR_LIGHT_CYAN,
        TERMINAL_COLOR_BLACK
    );

    terminal_write_line("TASOS v0.5.0");

    terminal_set_color(
        TERMINAL_COLOR_LIGHT_GREY,
        TERMINAL_COLOR_BLACK
    );

    terminal_write_line("Terminal driver initialized.");

    gdt_initialize();

    terminal_set_color(
        TERMINAL_COLOR_LIGHT_GREEN,
        TERMINAL_COLOR_BLACK
    );

    terminal_write_line("GDT initialized successfully.");

    idt_initialize();
    terminal_write_line("IDT initialized successfully.");

    irq_initialize();
    terminal_write_line("IRQ initialized successfully.");

    pic_initialize();
    terminal_write_line("PIC initialized suzzessfully.");

    keyboard_initialize();
    pic_unmask_irq(PIC_KEYBOARD_IRQ);
    terminal_write_line("Keyboard initialized successfully.");

    terminal_set_color(
        TERMINAL_COLOR_WHITE,
        TERMINAL_COLOR_BLACK
    );

    terminal_write_line("Kernel code segment: 0x08");
    terminal_write_line("Kernel data segment: 0x10");
    terminal_write_line("TASOS now owns its processor segmentation.");

    terminal_set_color(
        TERMINAL_COLOR_LIGHT_CYAN,
        TERMINAL_COLOR_BLACK
    );

    terminal_write_line("");
    terminal_write_line("You can start typing now...");

    __asm__ volatile ("sti");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}