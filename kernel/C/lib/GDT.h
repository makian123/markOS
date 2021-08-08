#ifndef GDT_H
#define GDT_H

#include "types.h"

struct GDTEntry
{
   uint16_t limit_low;           // The lower 16 bits of the limit.
   uint16_t base_low;            // The lower 16 bits of the base.
   uint8_t  base_middle;         // The next 8 bits of the base.
   uint8_t  access;              // Access flags, determine what ring this segment can be used in.
   uint8_t  granularity;
   uint8_t  base_high;           // The last 8 bits of the base.
} __attribute__((packed));

struct GDTR
{
   uint16_t limit;               // The upper 16 bits of all selector limits.
   uint64_t base;                // The address of the first gdt_entry_t struct.
}
 __attribute__((packed));

struct GDT{
    struct GDTEntry Null; //0x00
    struct GDTEntry KC; //0x08
    struct GDTEntry KD; //0x10
    struct GDTEntry UN;
    struct GDTEntry UC;
    struct GDTEntry UD;
}__attribute__((packed))
__attribute__((alligned(0x1000)));

struct GDT DefaultGDT;

extern void LoadGDT(struct GDTR *gdtr);

void GDTInit();

#endif