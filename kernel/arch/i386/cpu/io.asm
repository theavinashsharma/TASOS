bits 32

section .text

global port_in8
global port_in16
global port_out8
global port_out16
global io_wait

port_in8:
    mov dx, [esp + 4]
    xor eax, eax
    in al, dx
    ret

port_in16:
    mov dx, [esp + 4]
    xor eax, eax
    in ax, dx
    ret

port_out8:
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

port_out16:
    mov dx, [esp + 4]
    mov ax, [esp + 8]
    out dx, ax
    ret

io_wait:
    xor al, al
    out 0x80, al
    ret
