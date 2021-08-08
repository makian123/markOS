#ifndef IDT_H
#define IDT_H

#include "types.h"

typedef struct idtEntry{
    uint16_t baseLow;
    uint16_t kernelCS;
    uint8_t ist;
    uint8_t flags;
    uint16_t baseMid;
    uint32_t baseHigh;
    uint32_t reserved;
}__attribute__((packed));

typedef struct idtr{
    uint16_t limit;
    uint64_t base;
}__attribute__((packed));

typedef struct {
    uint64_t ds;
    uint64_t rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
    uint64_t intNo, errCode;
    uint64_t rip, cs, eflags, useresp, ss;
} registers;

void idtStart();
#endif