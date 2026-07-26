#ifndef TASOS_IRQ_H
#define TASOS_IRQ_H

#include <stdint.h>

#include <tasos/interrupt_frame.h>

#define IRQ_COUNT 16

typedef void (*irq_handler_function)(
    struct interrupt_frame* frame
);

void irq_initialize(void);

void irq_register_handler(
    uint8_t irq,
    irq_handler_function handler
);

void irq_unregister_handler(uint8_t irq);

void irq_dispatch(struct interrupt_frame* frame);

#endif
