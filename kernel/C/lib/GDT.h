#ifndef GDT_H
#define GDT_H

#include "types.h"
#include "cpu.h"

struct GDTDescriptor{
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

struct GDTEntry{
    uint16_t baseLow;
    uint16_t LimitLow;
    uint8_t baseMiddle;
    uint8_t Access;
    uint8_t Granularity;
    uint8_t baseHigh;
} __attribute__((packed));

bool GDTInit();

#endif