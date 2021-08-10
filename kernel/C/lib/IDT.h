#ifndef IDT_H
#define IDT_H

#include "types.h"

#define INTERRUPT_GATE 0x8E
#define TRAP_GATE 0xF

struct idt_64_entry
{
    uint16_t offset_1;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint32_t offset_2;
    uint32_t offset_3;
    uint32_t zero;
} __attribute__((packed));

struct idt_64_ptr
{
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

void idt_init();
void idt_flush();

#endif