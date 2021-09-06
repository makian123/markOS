#pragma once

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned long uintptr_t;
typedef uint64_t size_t;


#define null 0
#define NULL ((void*)null)

typedef struct BitmapStruct{
    uint8_t header[54];
    uint8_t *data;
}BITMAP;