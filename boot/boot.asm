bits 32

section .multiboot
align 4

MULTIBOOT_MAGIC    equ 0x1BADB002
MULTIBOOT_FLAGS    equ 0x00000000
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

dd MULTIBOOT_MAGIC
dd MULTIBOOT_FLAGS
dd MULTIBOOT_CHECKSUM

section .bss
align 16

stack_bottom:
    resb 16384
stack_top:

section .text
global _start
extern kernel_main

_start:
    mov esp, stack_top
    cli
    call kernel_main

.halt:
    hlt
    jmp .halt

section .note.GNU-stack noalloc noexec nowrite progbits