[bits 64]
global gdtFlush
extern gp

gdtFlush:
    lgdt [gp]
    mov ax, 0x10
    mov ds, ax ; should throw an error or crash
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    push 0x08
    push flush2
    retq

flush2:
    ret