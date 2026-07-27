#include <tasos/gdt.h>
#include <tasos/idt.h>
#include <tasos/terminal.h>
#include <tasos/irq.h>
#include <tasos/pic.h>
#include <tasos/pit.h>
#include <tasos/keyboard.h>

void kernel_main(void)
{
    terminal_initialize();

    terminal_set_color(
        TERMINAL_COLOR_LIGHT_CYAN,
        TERMINAL_COLOR_BLACK
    );

    terminal_write_line("TASOS v0.6.0");

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
    pit_initialize(PIT_DEFAULT_FREQUENCY_HZ);
    
    pic_unmask_irq(PIC_KEYBOARD_IRQ);
    pic_unmask_irq(PIC_TIMER_IRQ);
    
    
    terminal_write_line("Keyboard initialized successfully.");
    terminal_write_line("PIT initialized successfully.");
    terminal_write_line("Interrupts enabled.");
    terminal_write_line("");
    terminal_write_line("Type on the keyboard.");
    
    terminal_set_color(
        TERMINAL_COLOR_LIGHT_CYAN,
        TERMINAL_COLOR_BLACK
    );
    
    __asm__ volatile ("sti");

    uint64_t last_ticks = 0U;

    for (;;) {
        const uint64_t ticks = pit_get_ticks();

        if (ticks - last_ticks >= pit_get_frequency()) {
            last_ticks = ticks;
            terminal_write("Ticks: ");
            terminal_write_unsigned(
                (uint32_t)ticks
            );
            terminal_put_character('\n');
        }

        __asm__ volatile ("hlt");
    }
}