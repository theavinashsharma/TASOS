#ifndef TASOS_PIT_H
#define TASOS_PIT_H

#include <stdint.h>

#define PIT_DEFAULT_FREQUENCY_HZ 100U

void pit_initialize(uint32_t frequency_hz);

uint64_t pit_get_ticks(void);
uint32_t pit_get_frequency(void);
// uint64_t pit_get_uptime_seconds(void);
// uint64_t pit_get_uptime_milliseconds(void);

#endif
