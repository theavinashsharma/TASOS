#ifndef TASOS_PIC_H
#define TASOS_PIC_H

#include <stdint.h>

#define PIC_IRQ_COUNT 16

#define PIC_MASTER_VECTOR_OFFSET 32
#define PIC_SLAVE_VECTOR_OFFSET 40

#define PIC_TIMER_IRQ 0
#define PIC_KEYBOARD_IRQ 1
#define PIC_CASCADE_IRQ 2

void pic_initialize(void);

void pic_mask_irq(uint8_t irq);
void pic_unmask_irq(uint8_t irq);

void pic_mask_all(void);
void pic_send_end_of_interrupt(uint8_t irq);

#endif
