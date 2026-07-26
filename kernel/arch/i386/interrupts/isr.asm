bits 32

section .text

extern interrupt_handler

%macro ISR_NO_ERROR_CODE 1
global isr%1
isr%1:
    push dword 0
    push dword %1
    jmp isr_common
%endmacro

%macro ISR_ERROR_CODE 1
global isr%1
isr%1:
    push dword %1
    jmp isr_common
%endmacro

ISR_NO_ERROR_CODE 0
ISR_NO_ERROR_CODE 1
ISR_NO_ERROR_CODE 2
ISR_NO_ERROR_CODE 3
ISR_NO_ERROR_CODE 4
ISR_NO_ERROR_CODE 5
ISR_NO_ERROR_CODE 6
ISR_NO_ERROR_CODE 7

ISR_ERROR_CODE 8

ISR_NO_ERROR_CODE 9

ISR_ERROR_CODE 10
ISR_ERROR_CODE 11
ISR_ERROR_CODE 12
ISR_ERROR_CODE 13
ISR_ERROR_CODE 14

ISR_NO_ERROR_CODE 15
ISR_NO_ERROR_CODE 16

ISR_ERROR_CODE 17

ISR_NO_ERROR_CODE 18
ISR_NO_ERROR_CODE 19
ISR_NO_ERROR_CODE 20

ISR_ERROR_CODE 21

ISR_NO_ERROR_CODE 22
ISR_NO_ERROR_CODE 23
ISR_NO_ERROR_CODE 24
ISR_NO_ERROR_CODE 25
ISR_NO_ERROR_CODE 26
ISR_NO_ERROR_CODE 27
ISR_NO_ERROR_CODE 28

ISR_ERROR_CODE 29
ISR_ERROR_CODE 30

ISR_NO_ERROR_CODE 31

isr_common:
    cld
    pusha

    push esp
    call interrupt_handler
    add esp, 4


    popa


    add esp, 8
    iretd

section .rodata

global isr_stub_table

isr_stub_table:
    dd isr0
    dd isr1
    dd isr2
    dd isr3
    dd isr4
    dd isr5
    dd isr6
    dd isr7
    dd isr8
    dd isr9
    dd isr10
    dd isr11
    dd isr12
    dd isr13
    dd isr14
    dd isr15
    dd isr16
    dd isr17
    dd isr18
    dd isr19
    dd isr20
    dd isr21
    dd isr22
    dd isr23
    dd isr24
    dd isr25
    dd isr26
    dd isr27
    dd isr28
    dd isr29
    dd isr30
    dd isr31

section .note.GNU-stack noalloc noexec nowrite progbits
