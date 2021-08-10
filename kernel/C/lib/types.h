#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint64_t size_t;
typedef unsigned long uintptr_t;

#define LOW_8(address) (uint8_t)((address) & 0xFF)
#define HIGH_8(address) (uint8_t)(((address) >> 8) & 0xFF)
#define LOW_16(address) (uint16_t)((address) & 0xFFFF)
#define HIGH_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)
#define LOW_32(address) (uint32_t)((address) & 0xFFFFFFFF)
#define HIGH_32(address) (uint32_t)(((address) >> 32) & 0xFFFFFFFF)
#define LOW_64(address) (uint64_t)((address) & 0xFFFFFFFFFFFFFFFF)
#define HIGH_64(address) (uint64_t)(((address) >> 64) & 0xFFFFFFFFFFFFFFFF)

#define UNUSED(x) (void)(x)

//typedef uint32_t size_t;
typedef enum{
    false = (1 == 0),
    true = (! false)
} bool;


#define null 0
#define NULL ((void*)0))

typedef void* sasp_t;

#endif