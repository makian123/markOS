#ifndef MALLOC_H
#define MALLOC_H

#include "types.h"

#define MEMORY_SIZE 2000000

typedef struct block{
    size_t size;
    uint32_t free;
    struct block *next;
}block;

void Initialize();
void Split(struct block *fSlot, size_t size);
void *Malloc(size_t size);
void merge();
void Free(void *ptr);
void memcpy(void *dest, void *src, size_t n);
void *memset(void *dest, int val, size_t n);

#endif