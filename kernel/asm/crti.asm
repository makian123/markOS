section .init
global init

init:
    push rbp
    mov rbp, rsp

section .fini
global fini

fini:
    push rbp
    mov rbp, rsp