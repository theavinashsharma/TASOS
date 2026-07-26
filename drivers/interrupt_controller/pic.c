#include <stdint.h>

#include <tasos/io.h>
#include <tasos/pic.h>

#define PIC_MASTER_COMMAND_PORT 0x20
#define PIC_MASTER_DATA_PORT 0x21

#define PIC_SLAVE_COMMAND_PORT 0xA0
#define PIC_SLAVE_DATA_PORT 0xA1

#define PIC_END_OF_INTERRUPT 0x20

#define PIC_ICW1_INITIALIZE 0x10
#define PIC_ICW1_ICW4_NEEDED 0x01

#define PIC_ICW4_8086_MODE 0x01

#define PIC_MASTER_SLAVE_IRQ 0x04
#define PIC_SLAVE_ID 0x02

#define PIC_ALL_IRQS_MASKED 0XFF

static uint8_t master_mask = PIC_ALL_IRQS_MASKED;
static uint8_t slave_mask = PIC_ALL_IRQS_MASKED;

static void pic_write_masks(void)
{
    port_out8(PIC_MASTER_DATA_PORT, master_mask);
    port_out8(PIC_SLAVE_DATA_PORT, slave_mask);
}

void pic_mask_all(void)
{
    master_mask = PIC_ALL_IRQS_MASKED;
    slave_mask = PIC_ALL_IRQS_MASKED;

    pic_write_masks();
}

void pic_initialize(void)
{
    const uint8_t initialization_command =
        PIC_ICW1_INITIALIZE |
        PIC_ICW1_ICW4_NEEDED;

    /*
     * ICW1:
     * Begin the PIC initialization sequence. 
     */
    port_out8(PIC_MASTER_COMMAND_PORT, initialization_command);
    io_wait();

    port_out8(PIC_SLAVE_COMMAND_PORT, initialization_command);
    io_wait();

    /*
     * ICW2:
     * Configure interrupt-vector offsets.
     */
    port_out8(
        PIC_MASTER_DATA_PORT,
        PIC_MASTER_VECTOR_OFFSET
    );
    io_wait();

    port_out8(
        PIC_SLAVE_DATA_PORT,
        PIC_SLAVE_VECTOR_OFFSET
    );
    io_wait();

    /*
     * ICW3:
     * Tell the master that the slave is connected to IRQ2.
     * Tell the slave that its cascade identity is 2.
    */
   port_out8(
        PIC_MASTER_DATA_PORT,
        PIC_MASTER_SLAVE_IRQ
   );
   io_wait();

   port_out8(
        PIC_SLAVE_DATA_PORT,
        PIC_SLAVE_ID
   );
   io_wait();

   /*
    * ICW4:
    * Configure both PICs for 8086/32-bit protected-mode behavior.
    */
   port_out8(
        PIC_MASTER_DATA_PORT,
        PIC_ICW4_8086_MODE
   );
   io_wait();

   port_out8(
        PIC_SLAVE_DATA_PORT,
        PIC_ICW4_8086_MODE
   );
   io_wait();

   /*
    * Keep every IRQ masked until handlers have been installed.
    */
   pic_mask_all();
}

void pic_mask_irq(uint8_t irq)
{
    if (irq >= PIC_IRQ_COUNT) {
        return;
    }

    if (irq < 8U) {
        master_mask |= (uint8_t)(1U << irq);
        port_out8(PIC_MASTER_DATA_PORT, master_mask);
        return;
    }

    irq -= 8U;
    
    slave_mask |= (uint8_t)(1U << irq);
    port_out8(PIC_SLAVE_DATA_PORT, slave_mask);
}

void pic_unmask_irq(uint8_t irq)
{
    if (irq >= PIC_IRQ_COUNT) {
        return;
    }

    if (irq < 8U) {
        master_mask &= (uint8_t)~(1U << irq);
        port_out8(PIC_MASTER_DATA_PORT, master_mask);
        return;
    }

    irq -= 8U;

    slave_mask &= (uint8_t)~(1U << irq);
    port_out8(PIC_SLAVE_DATA_PORT, slave_mask);

    /*
     * Slave interrupts reach the cpu through master IRQ2,
     * so the cascade line must also be enabled.
    */
   master_mask &= (uint8_t)~(1U << PIC_CASCADE_IRQ);
   port_out8(PIC_MASTER_DATA_PORT, master_mask);
}

void pic_send_end_of_interrupt(uint8_t irq)
{
    if (irq >= PIC_IRQ_COUNT) {
        return;
    }

    /*
     * Slave IRQs must acknowledge both controllers.
    */
   if (irq >= 8U) {
        port_out8(
            PIC_SLAVE_COMMAND_PORT,
            PIC_END_OF_INTERRUPT
        );
   }

   port_out8(
        PIC_MASTER_COMMAND_PORT,
        PIC_END_OF_INTERRUPT
   );
}
