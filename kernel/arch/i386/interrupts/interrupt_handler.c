#include <stddef.h>
#include <stdint.h>

#include <tasos/interrupt_frame.h>
#include <tasos/terminal.h>

#define EXCEPTION_COUNT 32

static const char* const exception_names[EXCEPTION_COUNT] = {
    "Divide Error",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid Task State Segment",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved"
};

static void terminal_write_unsigned(uint32_t value)
{
    char digits[10];
    size_t digit_count = 0;

    if (value == 0U)
    {
        terminal_put_character('0');
        return;
    }

    while (value > 0U)
    {
        digits[digit_count] =
            (char)('0' + (value % 10U));

        digit_count++;
        value /= 10U;
    }

    while (digit_count > 0U)
    {
        digit_count--;
        terminal_put_character(digits[digit_count]);
    }
}

void interrupt_handler(struct interrupt_frame* frame)
{
    __asm__ volatile ("cli");

    terminal_set_color(
        TERMINAL_COLOR_WHITE,
        TERMINAL_COLOR_RED
    );

    terminal_write_line("");
    terminal_write_line(" TASOS KERNEL EXCEPTION ");

    terminal_set_color(
        TERMINAL_COLOR_LIGHT_RED,
        TERMINAL_COLOR_BLACK
    );

    terminal_write("Exception vector: ");
    terminal_write_unsigned(frame->vector);
    terminal_write_line("");

    terminal_write("Exception name: ");

    if (frame->vector < EXCEPTION_COUNT) {
        terminal_write_line(exception_names[frame->vector]);
    } else {
        terminal_write_line("Unknown");
    }

    terminal_write("Error code: ");
    terminal_write_unsigned(frame->error_code);
    terminal_write_line("");

    terminal_set_color(
        TERMINAL_COLOR_LIGHT_GREY,
        TERMINAL_COLOR_BLACK
    );

    terminal_write_line("Kernel execution halted safely.");

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
