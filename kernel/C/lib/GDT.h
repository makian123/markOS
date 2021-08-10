#ifndef GDT_H
#define GDT_H

#include "types.h"
#include "cpu.h"

struct GDTDescriptorStruct{
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));
typedef struct GDTDescriptorStruct gdt_ptr_t;

struct GDTEntryStruct{
    uint16_t baseLow;
    uint16_t LimitLow;
    uint8_t baseMiddle;
    uint8_t Access;
    uint8_t Granularity;
    uint8_t baseHigh;
} __attribute__((packed));
typedef struct GDTEntryStruct gdt_entry_t;

bool GDTInit();

#endif