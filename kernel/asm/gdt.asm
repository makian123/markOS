[bits 32]
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
    jmp 0x08:flush2
    ret

flush2:
    ret