#include <tasos/gdt.h>
#include <tasos/terminal.h>

void kernel_main(void)
{
    terminal_initialize();

    terminal_set_color(
        TERMINAL_COLOR_LIGHT_CYAN,
        TERMINAL_COLOR_BLACK
    );

    terminal_write_line("TASOS v0.2.0");

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

    terminal_write_line("GDT initialized successfully");

    terminal_set_color(
        TERMINAL_COLOR_WHITE,
        TERMINAL_COLOR_BLACK
    );

    terminal_write_line("Kernel code segment: 0x08");
    terminal_write_line("Kernel data segment: 0x10");
    terminal_write_line("TASOS now owns its processor segmentation.");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}