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

#endif