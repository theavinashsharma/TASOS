#include <stddef.h>
#include <stdint.h>

#include <tasos/interrupt_frame.h>
#include <tasos/irq.h>
#include <tasos/pic.h>

static irq_handler_function irq_handlers[IRQ_COUNT];

void irq_initialize(void)
{
    for (size_t irq = 0; irq < IRQ_COUNT; irq++) {
        irq_handlers[irq] = NULL;
    }
}

void irq_register_handler(
    uint8_t irq,
    irq_handler_function handler
)
{
    if (irq >= IRQ_COUNT) {
        return;
    }

    irq_handlers[irq] = handler;
}

void irq_unregister_handler(uint8_t irq)
{
    if (irq >= IRQ_COUNT)
    {
        return;
    }

    irq_handlers[irq] = NULL;
}

void irq_dispatch(struct interrupt_frame* frame)
{
    uint32_t irq;

    if (
        frame->vector < PIC_MASTER_VECTOR_OFFSET ||
        frame->vector >= PIC_MASTER_VECTOR_OFFSET + IRQ_COUNT
    ) {
        return;
    }

    irq = frame->vector - PIC_MASTER_VECTOR_OFFSET;

    if (irq_handlers[irq] != NULL) {
        irq_handlers[irq](frame);
    }

    pic_send_end_of_interrupt((uint8_t)irq);
}
