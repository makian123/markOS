#include "heap.hpp"
#include "pageTableManager.hpp"
#include "pageFrameAllocator.hpp"

void *heapStart;
void *heapEnd;
HeapSegmentHdr *lastHdr;

void initHeap(void *heapAddr, size_t heapLen){
    void *pos = heapAddr;

    for(size_t i = 0; i < heapLen; ++i){
        globalPageTableManager.MapMemory(pos, globalAllocator.RequestPage());
        pos = (void*)((size_t)pos + 0x1000);
    }

    size_t len = heapLen * 0x1000;
    heapStart = heapAddr;
    heapEnd = (void*)((size_t)heapStart + heapLen);
    HeapSegmentHdr *startSegment = (HeapSegmentHdr*)heapAddr;
    startSegment->len = heapLen - sizeof(HeapSegmentHdr);
    startSegment->next = NULL;
    startSegment->last = NULL;
    startSegment->free = true;
    lastHdr = startSegment;
}

void *malloc(size_t size){
    if(size % 0x10 > 0){
        size -= (size % 10);
        size += 0x10;
    }
    if(size == 0) return NULL;

    HeapSegmentHdr *currSeg = (HeapSegmentHdr*)heapStart;
    while(true){    
        if(currSeg->free){
            if(currSeg->len > size){
                currSeg->split(size);
                currSeg->free = false;
                return (void*)((uint64_t)currSeg + sizeof(HeapSegmentHdr));
            }

            if(currSeg->len == size){
                currSeg->free = false;
                return (void*)((uint64_t)currSeg + sizeof(HeapSegmentHdr));
            }
        }
        if(currSeg->next == NULL) break;
        currSeg = currSeg->next;
    }

    expandHeap(size);
    return malloc(size);
}

HeapSegmentHdr *HeapSegmentHdr::split(size_t splitLen){
    if(splitLen < 0x10) return NULL;
    long splitSegLen = len - splitLen - sizeof(HeapSegmentHdr);
    if(splitSegLen < 0x10) return NULL;

    HeapSegmentHdr *newSplit = (HeapSegmentHdr*)((size_t)this + splitLen + sizeof(HeapSegmentHdr));
    next->last = newSplit;
    newSplit->next = next;
    next = newSplit;
    newSplit->last = this;
    newSplit->len = splitSegLen;
    newSplit->free = free;
    len = splitLen;

    if(lastHdr == this) lastHdr = newSplit;
    return newSplit;
}

void free(void *addr){
    HeapSegmentHdr *segm = (HeapSegmentHdr*)addr - 1;

    segm->free = true;
    segm->CombineFwd();
    segm->CombineBwd();
}

void expandHeap(size_t size){
    if(size % 0x1000){
        size -= (size % 1000);
        size += 0x1000;
    }

    size_t pageCount = size / 0x1000;
    HeapSegmentHdr *newSegment = (HeapSegmentHdr*)heapEnd;
    for(size_t i = 0; i < pageCount; ++i){
        globalPageTableManager.MapMemory(heapEnd, globalAllocator.RequestPage());
        heapEnd = (void*)((size_t)heapEnd + 0x1000);
    }

    newSegment->free = true;
    newSegment->last = lastHdr;
    lastHdr->next = newSegment;
    lastHdr = newSegment;
    newSegment->next = NULL;
    newSegment->len = size - sizeof(HeapSegmentHdr);
    newSegment->CombineBwd();
}

void HeapSegmentHdr::CombineFwd(){
    if(next == NULL) return;
    if(!next->free) return;

    if(next == lastHdr) lastHdr = this;
    
    if(next->next != NULL) next->next->last = this;

    len = len + next->len + sizeof(HeapSegmentHdr);
}

void HeapSegmentHdr::CombineBwd(){
    if(last != NULL && last-free) last->CombineFwd();
}