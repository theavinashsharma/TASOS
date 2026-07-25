#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY_ADDRESS 0xB8000

static volatile uint16_t *const vga_memory = (volatile uint16_t*)VGA_MEMORY_ADDRESS;

static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0x0F;

static uint16_t vga_entry(char character, uint8_t color)
{
    return (uint16_t)character | ((uint16_t)color << 8);
}

static void terminal_clear(void)
{
    for(size_t row = 0; row < VGA_HEIGHT; row++)
    {
        for(size_t col = 0; col < VGA_WIDTH; col++)
        {
            const size_t index = row * VGA_WIDTH + col;
            vga_memory[index] = vga_entry(' ', terminal_color);
        }
    }

    terminal_row = 0;
    terminal_column = 0;
}

static void terminal_put_character(char character)
{
    if (character == '\n')
    {
        terminal_column = 0;
        terminal_row++;

        if (terminal_row >= VGA_HEIGHT)
        {
            terminal_row = 0;
        }

        return;
    }

    const size_t index = terminal_row * VGA_WIDTH + terminal_column;
    vga_memory[index] = vga_entry(character, terminal_color);
    terminal_column++;

    if (terminal_column >= VGA_WIDTH)
    {
        terminal_column = 0;
        terminal_row++;
    }

    if (terminal_row >= VGA_HEIGHT)
    {
        terminal_row = 0;
    }
}

static void terminal_write(const char* text)
{
    for (size_t index = 0; text[index] != '\0'; index++)
    {
        terminal_put_character(text[index]);
    }
}

void kernel_main(void)
{
    terminal_clear();

    terminal_write("TASOS v0.1.0\n");
    terminal_write("Kernel initialized successfully.\n");
    terminal_write("Welcome to The Avinash Sharma OS (TASOS)!\n");

    for (;;) 
    {
        __asm__ volatile ("hlt");
    }
}