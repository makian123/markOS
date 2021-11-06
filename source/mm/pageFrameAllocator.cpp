#include "pageFrameAllocator.hpp"
#include "mem.hpp"

#define PAGE_SIZE 4096

memInfo RAM = { 0, 0, 0, 0 };
bool initialized = false;
PageFrameAllocator globalAllocator;

void PageFrameAllocator::Initialize(stivale2_struct_tag_memmap *memoryMap){
    if(initialized) return;
    initialized = true;

    void *largestMemSeg = NULL;
    size_t largestMemSegSize = 0;

    for(uint64_t i = 0; i < memoryMap->entries; ++i){
        stivale2_mmap_entry desc = memoryMap->memmap[i];

        if(desc.type == STIVALE2_MMAP_USABLE){
            if(desc.length * PAGE_SIZE > largestMemSegSize){
                largestMemSeg = (void*)desc.base;
                largestMemSegSize = desc.length * 4096;
            }
        }
    }

    uint64_t memSize = getMemSize(memoryMap);
    RAM.freeMemory = memSize;
    RAM.totalMemory = memSize;

    uint64_t bitmapSize = memSize / 4096 / 8 + 1;

    initBitmap(largestMemSeg, bitmapSize);

    LockPages(pageBitmap.Buffer, pageBitmap.Size / PAGE_SIZE + 1);

    for(uint64_t i = 0; i < memoryMap->entries; ++i){
        stivale2_mmap_entry desc = memoryMap->memmap[i];

        if(desc.type != STIVALE2_MMAP_USABLE){
            ReservePages((void*)desc.base, desc.length / PAGE_SIZE);
        }
    }
}

void PageFrameAllocator::initBitmap(void *buff, size_t size){
    pageBitmap.Size = size;
    pageBitmap.Buffer = (uint8_t*)buff;

    for(uint64_t i = 0; i < size; ++i){
        *(uint8_t*)(pageBitmap.Buffer + i) = 0;
    }
}

uint64_t pageBitmapIndex = 0;
void PageFrameAllocator::FreePage(void *addr){
    uint64_t index = (uint64_t)addr / PAGE_SIZE;

    if(pageBitmap[index] == false) return;

    if(pageBitmap.Set(index, false)){
        RAM.freeMemory += PAGE_SIZE;
        RAM.usedMemory -= PAGE_SIZE;
        if(pageBitmapIndex > index) pageBitmapIndex = index;
    }
}
void PageFrameAllocator::FreePages(void *buff, size_t pages){
    for(size_t i = 0; i < pages; ++i){
        FreePage((void*)((uint64_t)buff + (i * 4096)));
    }
}

void PageFrameAllocator::LockPage(void *addr){
    uint64_t index = (uint64_t)addr / PAGE_SIZE;

    if(pageBitmap[index] == true) return;

    if(pageBitmap.Set(index, true)){
        RAM.freeMemory -= PAGE_SIZE;
        RAM.usedMemory += PAGE_SIZE;
    }
}
void PageFrameAllocator::LockPages(void *buff, size_t pages){
    for(size_t i = 0; i < pages; ++i){
        LockPage((void*)((uint64_t)buff + (i * 4096)));
    }
}

void PageFrameAllocator::ReservePage(void *addr){
    uint64_t index = (uint64_t)addr / PAGE_SIZE;

    if(pageBitmap[index] == true) return;

    if(pageBitmap.Set(index, true)){
        RAM.reservedMemory -= PAGE_SIZE;
        RAM.usedMemory += PAGE_SIZE;
    }
}
void PageFrameAllocator::ReservePages(void *buff, size_t pages){
    for(size_t i = 0; i < pages; ++i){
        ReservePage((void*)((uint64_t)buff + (i * 4096)));
    }
}

void PageFrameAllocator::UnreservePage(void *addr){
    uint64_t index = (uint64_t)addr / PAGE_SIZE;

    if(pageBitmap[index] == false) return;

    if(pageBitmap.Set(index, false)){
        RAM.reservedMemory += PAGE_SIZE;
        RAM.usedMemory -= PAGE_SIZE;
        if(pageBitmapIndex > index) pageBitmapIndex = index;
    }
}
void PageFrameAllocator::UnreservePages(void *buff, size_t pages){
    for(size_t i = 0; i < pages; ++i){
        UnreservePage((void*)((uint64_t)buff + (i * 4096)));
    }
}

void *PageFrameAllocator::RequestPage(){
    for(; pageBitmapIndex < pageBitmap.Size * 8; ++pageBitmapIndex){
        if(pageBitmap[pageBitmapIndex] == true) continue;

        LockPage((void*)(pageBitmapIndex * 4096));
        return (void*)(pageBitmapIndex * 4096);
    }

    return NULL;
}

memInfo PageFrameAllocator::getRAM(){
    return RAM;
}