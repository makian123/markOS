#ifndef IDT_H
#define IDT_H

#include "types.h"

typedef struct{
    uint16_t isr_low;
    uint16_t kernel_cs;
    uint8_t ist;
    uint8_t attributes;
    uint16_t isr_mid;
    uint32_t isr_high;
    uint32_t reserved;
} __attribute__((packed)) idt_entry_t;

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];

typedef struct {
	uint16_t	limit;
	uint64_t	base;
} __attribute__((packed)) idtr_t;
static idtr_t idtr;


typedef struct {
   uint16_t base_low;
   uint16_t cs;
   uint8_t ist;
   uint8_t attributes;
   uint16_t base_mid;
   uint32_t base_high;
   uint32_t rsv0;
} idt_desc_t;

extern void* isr_stub_table[];
bool IDTInit(void);

#endif