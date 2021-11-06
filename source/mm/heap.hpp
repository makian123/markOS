#pragma once

#include <include/types.hpp>

struct HeapSegmentHdr{
    size_t len;
    HeapSegmentHdr *next;
    HeapSegmentHdr *last;
    bool free;
    void CombineFwd();
    void CombineBwd();
    HeapSegmentHdr *split(size_t splitLen);
};

void initHeap(void *heapAddr, size_t heapLen);

void *malloc(size_t size);
void free(void *addr);

void expandHeap(size_t size);