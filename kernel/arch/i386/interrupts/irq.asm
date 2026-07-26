bits 32

section .text

extern irq_dispatch

%macro IRQ_STUB 2
global irq%1
irq%1:
    push dword 0
    push dword %2
    jmp irq_common
%endmacro

IRQ_STUB 0, 32
IRQ_STUB 1, 33
IRQ_STUB 2, 34
IRQ_STUB 3, 35
IRQ_STUB 4, 36
IRQ_STUB 5, 37
IRQ_STUB 6, 38
IRQ_STUB 7, 39
IRQ_STUB 8, 40
IRQ_STUB 9, 41
IRQ_STUB 10, 42
IRQ_STUB 11, 43
IRQ_STUB 12, 44
IRQ_STUB 13, 45
IRQ_STUB 14, 46
IRQ_STUB 15, 47

irq_common:
    cld
    pusha

    push esp
    call irq_dispatch
    add esp, 4

    popa

    add esp, 8
    iretd

section .rodata

global irq_stub_table

irq_stub_table:
    dd irq0
    dd irq1
    dd irq2
    dd irq3
    dd irq4
    dd irq5
    dd irq6
    dd irq7
    dd irq8
    dd irq9
    dd irq10
    dd irq11
    dd irq12
    dd irq13
    dd irq14
    dd irq15

section .note.GNU-stack noalloc noexec nowrite progbits
