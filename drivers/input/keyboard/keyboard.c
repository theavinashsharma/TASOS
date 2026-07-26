#include <stdbool.h>
#include <stdint.h>

#include <tasos/interrupt_frame.h>
#include <tasos/io.h>
#include <tasos/irq.h>
#include <tasos/keyboard.h>
#include <tasos/pic.h>
#include <tasos/terminal.h>

#define KEYBOARD_DATA_PORT 0x60

#define KEY_RELEASED_MASK 0x80

#define SCANCODE_LEFT_SHIFT  0x2A
#define SCANCODE_RIGHT_SHIFT 0x36

#define SCANCODE_BACKSPACE 0x0E
#define SCANCODE_ENTER     0x1C
#define SCANCODE_SPACE     0x39

#define SCANCODE_TABLE_SIZE 128

static bool left_shift_pressed;
static bool right_shift_pressed;

static const char normal_scancode_table[SCANCODE_TABLE_SIZE] = {
    [0x01] = 0,
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0A] = '9',
    [0x0B] = '0',
    [0x0C] = '-',
    [0x0D] = '=',
    [0x0E] = '\b',
    [0x0F] = '\t',

    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',
    [0x1A] = '[',
    [0x1B] = ']',
    [0x1C] = '\n',

    [0x1E] = 'a',
    [0x1F] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',
    [0x27] = ';',
    [0x28] = '\'',
    [0x29] = '`',

    [0x2B] = '\\',

    [0x2C] = 'z',
    [0x2D] = 'x',
    [0x2E] = 'c',
    [0x2F] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',
    [0x33] = ',',
    [0x34] = '.',
    [0x35] = '/',

    [0x39] = ' '
};

static const char shifted_scancode_table[SCANCODE_TABLE_SIZE] = {
    [0x01] = 0,
    [0x02] = '!',
    [0x03] = '@',
    [0x04] = '#',
    [0x05] = '$',
    [0x06] = '%',
    [0x07] = '^',
    [0x08] = '&',
    [0x09] = '*',
    [0x0A] = '(',
    [0x0B] = ')',
    [0x0C] = '_',
    [0x0D] = '+',
    [0x0E] = '\b',
    [0x0F] = '\t',

    [0x10] = 'Q',
    [0x11] = 'W',
    [0x12] = 'E',
    [0x13] = 'R',
    [0x14] = 'T',
    [0x15] = 'Y',
    [0x16] = 'U',
    [0x17] = 'I',
    [0x18] = 'O',
    [0x19] = 'P',
    [0x1A] = '{',
    [0x1B] = '}',
    [0x1C] = '\n',

    [0x1E] = 'A',
    [0x1F] = 'S',
    [0x20] = 'D',
    [0x21] = 'F',
    [0x22] = 'G',
    [0x23] = 'H',
    [0x24] = 'J',
    [0x25] = 'K',
    [0x26] = 'L',
    [0x27] = ':',
    [0x28] = '"',
    [0x29] = '~',

    [0x2B] = '|',

    [0x2C] = 'Z',
    [0x2D] = 'X',
    [0x2E] = 'C',
    [0x2F] = 'V',
    [0x30] = 'B',
    [0x31] = 'N',
    [0x32] = 'M',
    [0x33] = '<',
    [0x34] = '>',
    [0x35] = '?',

    [0x39] = ' '
};

static bool keyboard_shift_pressed(void)
{
    return left_shift_pressed || right_shift_pressed;
}

static void keyboard_handle_key_release(uint8_t scancode)
{
    const uint8_t make_code =
        (uint8_t)(scancode & ~KEY_RELEASED_MASK);

    if (make_code == SCANCODE_LEFT_SHIFT) {
        left_shift_pressed = false;
        return;
    }

    if (make_code == SCANCODE_RIGHT_SHIFT) {
        right_shift_pressed = false;
    }
}

static void keyboard_handle_special_key(uint8_t scancode)
{
    if (scancode == SCANCODE_BACKSPACE) {
        terminal_put_character('\b');
        return;
    }

    if (scancode == SCANCODE_ENTER) {
        terminal_put_character('\n');
        return;
    }

    if (scancode == SCANCODE_SPACE) {
        terminal_put_character(' ');
    }
}

static void keyboard_handle_key_press(uint8_t scancode)
{
    char character;

    if (scancode == SCANCODE_LEFT_SHIFT) {
        left_shift_pressed = true;
        return;
    }

    if (scancode == SCANCODE_RIGHT_SHIFT) {
        right_shift_pressed = true;
        return;
    }

    if (
        scancode == SCANCODE_BACKSPACE ||
        scancode == SCANCODE_ENTER ||
        scancode == SCANCODE_SPACE
    ) {
        keyboard_handle_special_key(scancode);
        return;
    }

    if (scancode >= SCANCODE_TABLE_SIZE) {
        return;
    }

    if (keyboard_shift_pressed()) {
        character = shifted_scancode_table[scancode];
    } else {
        character = normal_scancode_table[scancode];
    }

    if (character != 0) {
        terminal_put_character(character);
    }
}

static void keyboard_interrupt_handler(
    struct interrupt_frame* frame
)
{
    uint8_t scancode;

    (void)frame;

    scancode = port_in8(KEYBOARD_DATA_PORT);

    if ((scancode & KEY_RELEASED_MASK) != 0U) {
        keyboard_handle_key_release(scancode);
        return;
    }

    keyboard_handle_key_press(scancode);
}

void keyboard_initialize(void)
{
    left_shift_pressed = false;
    right_shift_pressed = false;

    irq_register_handler(
        PIC_KEYBOARD_IRQ,
        keyboard_interrupt_handler
    );
}
