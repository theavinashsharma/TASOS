bits 32

section .text

global gdt_flush

gdt_flush:
    mov eax, [esp + 4]
    lgdt [eax]

    movzx eax, word [esp + 12]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    movzx eax, word [esp + 8]
    push eax
    push dword .reload_code_segment
    retf

.reload_code_segment:
    ret
    
section .note.GNU-stack noalloc noexec nowrite progbits