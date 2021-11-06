#pragma once

#include <include/types.hpp>
#include <include/bitmap.hpp>
#include <stivale2.h>

struct memInfo{
    uint64_t freeMemory;
    uint64_t totalMemory;
    uint64_t usedMemory;
    uint64_t reservedMemory;
};

class PageFrameAllocator{
    public:
        void Initialize(stivale2_struct_tag_memmap *memoryMap);
        Bitmap pageBitmap;

        void FreePage(void *addr);
        void FreePages(void *addr, size_t pages);
        void LockPage(void *addr);
        void LockPages(void *addr, size_t pages);
        void ReservePage(void *addr);
        void ReservePages(void *addr, size_t pages);
        void UnreservePage(void *addr);
        void UnreservePages(void *addr, size_t pages);

        void *RequestPage();

        memInfo getRAM();

    private:
        void initBitmap(void *buff, size_t size);
};

extern PageFrameAllocator globalAllocator;