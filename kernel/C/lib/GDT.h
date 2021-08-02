#ifndef GDT_H
#define GDT_H

#include "types.h"

struct GDTDescriptor {
    uint16_t Size;
    uint64_t Offset;
} __attribute__((packed));

struct GDTEntry {
    uint16_t Limit0;
    uint16_t Base0;
    uint8_t Base1;
    uint8_t AccessByte;
    uint8_t Limit1_Flags;
    uint8_t Base2;
}__attribute__((packed));

struct GDT {
    struct GDTEntry Null;
    struct GDTEntry KernelCode;
    struct GDTEntry KernelData;
    struct GDTEntry UserNull;
    struct GDTEntry UserCode;
    struct GDTEntry UserData;
    struct GDTEntry Reserved;
} __attribute__((packed)) 

__attribute((aligned(0x1000)));

extern struct GDT DefaultGDT;
extern void LoadGDT(struct GDTDescriptor *gdtDescriptor);
bool GDTInit();

#endif