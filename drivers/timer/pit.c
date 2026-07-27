#include <stdint.h>

#include <tasos/interrupt_frame.h>
#include <tasos/io.h>
#include <tasos/irq.h>
#include <tasos/pic.h>
#include <tasos/pit.h>

#define PIT_INPUT_FREQUENCY_HZ 1193182U

#define PIT_CHANNEL_0_DATA_PORT 0x40U
#define PIT_COMMAND_PORT 0x43U

#define PIT_CHANNEL_0_SELECT 0x00U
#define PIT_ACCESS_LOW_HIGH 0x30U
#define PIT_MODE_3 0x06U
#define PIT_BINARY_MODE 0x00U

#define PIT_MIN_DIVISOR 1U
#define PIT_MAX_DIVISOR 65535U

static volatile uint64_t pit_ticks;
static uint32_t pit_frequency_hz;

static uint32_t pit_calculate_divisor(uint32_t frequency_hz)
{
    uint32_t divisor;

    divisor = PIT_INPUT_FREQUENCY_HZ / frequency_hz;

    if (divisor < PIT_MIN_DIVISOR) {
        divisor = PIT_MIN_DIVISOR;
    }

    if (divisor > PIT_MAX_DIVISOR) {
        divisor = PIT_MAX_DIVISOR;
    }

    return divisor;
}

static void pit_interrupt_handler(
    struct interrupt_frame* frame
)
{
    (void)frame;
    pit_ticks++;
}

static void pit_program_hardware(uint32_t divisor)
{
    const uint8_t command =
        PIT_CHANNEL_0_SELECT |
        PIT_ACCESS_LOW_HIGH |
        PIT_MODE_3 |
        PIT_BINARY_MODE;

    const uint8_t divisor_low =
        (uint8_t)(divisor & 0xFFU);

    const uint8_t divisor_high =
        (uint8_t)((divisor >> 8U) & 0xFFU);

    port_out8(PIT_COMMAND_PORT, command);

    port_out8(
        PIT_CHANNEL_0_DATA_PORT,
        divisor_low
    );

    port_out8(
        PIT_CHANNEL_0_DATA_PORT,
        divisor_high
    );
}

void pit_initialize(uint32_t frequency_hz)
{
    uint32_t divisor;

    if (frequency_hz == 0U)
    {
        frequency_hz = PIT_DEFAULT_FREQUENCY_HZ;
    }

    divisor = pit_calculate_divisor(frequency_hz);

    /*
     * Store the frequency the PIT can actually generate rather
     * than blindly preserving the requested value.
    */

    pit_frequency_hz = PIT_INPUT_FREQUENCY_HZ / divisor;

    pit_ticks = 0U;

    irq_register_handler(
    PIC_TIMER_IRQ,
    pit_interrupt_handler
    );

    pit_program_hardware(divisor);
}

uint64_t pit_get_ticks(void)
{
    uint32_t saved_eflags;
    uint64_t ticks;

    /*
    * Reading a 64-bit value is not atomic on a 32-bit CPU.
    * Temporarily disable interrupts so the PIT handler cannot
    * update pit_ticks halfway through the read.
    */
    __asm__ volatile (
        "pushf\n"
        "pop %0\n"
        "cli"
        : "=r"(saved_eflags)
        :
        : "memory"
    );

    ticks = pit_ticks;

    /*
    * Restore the previous interrupt-enable state rather than
    * always executing STI
    */
    if ((saved_eflags & (1U << 9U))!=0U) {
        __asm__ volatile(
            "sti"
            :
            :
            : "memory"
        );
    }

    return ticks;
}

uint32_t pit_get_frequency(void)
{
    return pit_frequency_hz;
}

// uint64_t pit_get_uptime_seconds(void)
// {
//     const uint32_t frequency_hz =
//         pit_get_frequency();

//     const uint64_t ticks =
//         pit_get_ticks();

//     if (frequency_hz == 0U) {
//         return 0U;
//     }

//     return ticks / frequency_hz;
// }

// uint64_t pit_get_uptime_milliseconds(void)
// {
//     const uint32_t frequency_hz = pit_get_frequency();

//     if (frequency_hz == 0U)
//     {
//         return 0U;
//     }

//     return (
//         pit_get_ticks() * 1000U
//     ) / frequency_hz;
// }
