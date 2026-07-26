#include <tasos/terminal.h>

void kernel_main(void)
{
    terminal_initialize();

    terminal_set_color(
        TERMINAL_COLOR_LIGHT_CYAN,
        TERMINAL_COLOR_BLACK
    );

    terminal_write_line("TASOS v0.1.0");

    terminal_set_color(
        TERMINAL_COLOR_LIGHT_GREEN,
        TERMINAL_COLOR_BLACK
    );

    terminal_write_line("Kernel initialized successfully.");

    terminal_set_color(
        TERMINAL_COLOR_WHITE,
        TERMINAL_COLOR_BLACK
    );

    terminal_write_line("Welcome, Avinash.");
    terminal_write_line("");

    terminal_set_color(
        TERMINAL_COLOR_LIGHT_GREY,
        TERMINAL_COLOR_BLACK
    );

    terminal_write_line("Terminal driver initialized.");
    terminal_write_line("Testing screen scrolling...");

    for (int line = 1; line <= 30; line++) {
        terminal_write("Terminal test line ");
        terminal_put_character(
            (char)('0' + ((line / 10) % 10))
        );
        terminal_put_character(
            (char)('0' + (line % 10))
        );
        terminal_write_line("");
    }

    for (;;) {
        __asm__ volatile ("hlt");
    }
}