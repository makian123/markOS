#pragma once

#include "paging.hpp"
#include <include/types.hpp>

class PageTableManager{
    public:
        PageTableManager(PageTable *PML4Address);
        PageTable *PML4Addr;
        void MapMemory(void *virt, void *phys);
};

extern PageTableManager globalPageTableManager;