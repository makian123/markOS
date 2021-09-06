#pragma once

#include <include/types.hpp>

struct ataIdentity{
    uint16_t type;
    uint8_t unused1[52];
    char model[40];
    uint8_t unused2[4];
    uint16_t capabilities;
    uint8_t unused3[20];
    uint16_t max_lba;
    uint8_t unused4[76];
    uint16_t max_lba_ext;
    uint8_t unused5[304];
}__attribute__((packed));

struct ataDevice{
    uint32_t base;
    uint32_t control;
    uint8_t slave;
    uint8_t isAtapi;
    ataIdentity identity;
};

void initATA();

ataDevice *getDevice(int index);
void ataReadsector(ataDevice *device, uint64_t lba, void* buffer);
void ataWriteSector(ataDevice *device, uint64_t lba, const void* buffer);