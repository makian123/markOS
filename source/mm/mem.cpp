#include "mem.hpp"

void *memset(void *dest, int val, size_t len){
    uint8_t *pointer = (uint8_t*)dest;
    while(len > 0){
        *pointer = val;
        pointer++;
        len--;
    }

    return dest;
}

void *memcpy(void *dest, void *src, size_t len){
    uint8_t *pSrc = (uint8_t*)src;
    uint8_t *pDest = (uint8_t*)dest;

    len--;
    while(len >= 0){
        pDest[len] = pSrc[len];
        len--;
    }

    return dest;
}

int memcmp(void *first, void *second, size_t len){
    uint8_t *f = (uint8_t*)first;
    uint8_t *s = (uint8_t*)second;
    
    for(size_t i = 0; i < len; ++i){
        if(f[i] < s[i]) return -1;
        if(s[i] < f[i]) return 1;
    }

    return 0;
}

uint64_t getMemSize(stivale2_struct_tag_memmap *memmap){
    static uint64_t memBytes = 0;
    if (memBytes > 0) return memBytes;

    for (int i = 0; i < memmap->entries; i++)
    {
        if(memmap->memmap[i].type == STIVALE2_MMAP_USABLE)
            memBytes += memmap->memmap[i].length;
    }

    return memBytes;
}