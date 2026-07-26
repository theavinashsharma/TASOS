#ifndef TASOS_IO_H
#define TASOS_IO_H

#include <stdint.h>

uint8_t port_in8(uint16_t port);
uint16_t port_in16(uint16_t port);

void port_out8(uint16_t port, uint8_t value);
void port_out16(uint16_t port, uint16_t value);

void io_wait(void);

#endif
