#ifndef IDT_H
#define IDT_H

#include "types.h"

struct IDTDescriptorStruct{
    uint16_t BaseLow;   //Offset bits 0-15
    uint16_t Selector;  //Code segment selector in GDT
    uint8_t IST;    //Offset of IST
    uint8_t Attr;   //Type and attributes
    uint16_t BaseMiddle;   //Offset bits 16-31
    uint32_t BaseHigh;   //Offset bits 32-63
    uint32_t Zero;  //Reserved
}__attribute__((packed));
typedef struct IDTDescriptorStruct idt_t;

struct IDTPointerStruct{
    uint16_t size;
    uint64_t offset;
}__attribute__((packed));
typedef struct IDTPointerStruct idtr_t;

bool idtInit();
void idtSet(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags);
#endif