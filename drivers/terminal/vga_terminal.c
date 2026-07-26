#include <stddef.h>
#include <stdint.h>

#include <tasos/terminal.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY_ADDRESS 0xB8000

static volatile uint16_t* const vga_memory = 
    (volatile uint16_t*) VGA_MEMORY_ADDRESS;

static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0;

static uint8_t terminal_make_color(
    enum terminal_color foreground,
    enum terminal_color background
)
{
    return (uint8_t)foreground | ((uint8_t)background << 4);
}

static uint16_t terminal_make_entry(char character, uint8_t color)
{
    return (uint16_t)(uint8_t)character | ((uint16_t)color << 8);
}

static void terminal_write_cell(
    size_t row,
    size_t column,
    char character,
    uint8_t color
)
{
    const size_t index = row * VGA_WIDTH + column;
    vga_memory[index] = terminal_make_entry(character, color);
}

static void terminal_scroll(void)
{
    for (size_t row = 1; row < VGA_HEIGHT; row++) {
        for (size_t column = 0; column < VGA_WIDTH; column++) {
            const size_t source_index = row * VGA_WIDTH + column;
            const size_t destination_index = 
                (row - 1) * VGA_WIDTH + column;

            vga_memory[destination_index] = vga_memory[source_index];
        }
    }

    for (size_t column = 0; column < VGA_WIDTH; column++) {
        terminal_write_cell(
            VGA_HEIGHT - 1,
            column,
            ' ',
            terminal_color
        );
    }

    terminal_row = VGA_HEIGHT - 1;
}

static void terminal_advance_line(void)
{
    terminal_column = 0;
    terminal_row++;

    if (terminal_row >=VGA_HEIGHT) {
        terminal_scroll();
    }
}

void terminal_initialize(void)
{
    terminal_color = terminal_make_color(
        TERMINAL_COLOR_LIGHT_GREY,
        TERMINAL_COLOR_BLACK
    );

    terminal_clear();
}

void terminal_clear(void)
{
    for (size_t row = 0; row < VGA_HEIGHT; row ++) {
        for (size_t column = 0; column < VGA_WIDTH; column++) {
            terminal_write_cell(row, column, ' ', terminal_color);
        }
    }

    terminal_row = 0;
    terminal_column = 0;
}

void terminal_set_color(
    enum terminal_color foreground,
    enum terminal_color background
)
{
    terminal_color = terminal_make_color(foreground, background);
}

void terminal_put_character(char character)
{
    if (character == '\n') {
        terminal_advance_line();
        return;
    }

    terminal_write_cell(
        terminal_row,
        terminal_column,
        character,
        terminal_color
    );

    terminal_column++;

    if (terminal_column >= VGA_WIDTH) {
        terminal_advance_line();
    }
}

void terminal_write(const char* text)
{
    if (text == NULL) {
        return;
    }

    for (size_t index = 0; text[index] != '\0'; index++) {
        terminal_put_character(text[index]);
    }
}

void terminal_write_line(const char* text)
{
    terminal_write(text);
    terminal_put_character('\n');
}