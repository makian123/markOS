global kernel_init
extern kmain
extern init

section .text
bits 64

kernel_init:
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call init
    
    call kmain
    cli
.hang:
    hlt
    jmp .hang