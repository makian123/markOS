#include "pageTableManager.hpp"
#include <include/types.hpp>
#include "pageFrameAllocator.hpp"
#include "pageMapIndexer.hpp"
#include "mem.hpp"

#define PAGE_SIZE 4096

PageTableManager globalPageTableManager = *(PageTableManager*)NULL;

PageTableManager::PageTableManager(PageTable *PML4Address){
    PML4Addr = PML4Address;
}

void PageTableManager::MapMemory(void *virt, void *phys){
    PageMapIndexer indexer = PageMapIndexer((uint64_t)virt);

    PageDirEntry PDE;

    PDE = PML4Addr->entries[indexer.PDP_i];
    PageTable *PDP;
    if(!PDE.GetFlag(PT_Flags::Present)){
        PDP = (PageTable*)globalAllocator.RequestPage();
        memset(PDP, 0, PAGE_SIZE);
        PDE.SetAddress((uint64_t)PDP >> 12);
        PDE.SetFlag(PT_Flags::Present, true);
        PDE.SetFlag(PT_Flags::RW, true);
        PML4Addr->entries[indexer.PDP_i] = PDE;
    }
    else PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);

    PDE = PDP->entries[indexer.PD_i];
    PageTable *PD;
    if(!PDE.GetFlag(PT_Flags::Present)){
        PD = (PageTable*)globalAllocator.RequestPage();
        memset(PD, 0, PAGE_SIZE);
        PDE.SetAddress((uint64_t)PD >> 12);
        PDE.SetFlag(PT_Flags::Present, true);
        PDE.SetFlag(PT_Flags::RW, true);
        PDP->entries[indexer.PD_i] = PDE;
    }
    else PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);

    PDE = PD->entries[indexer.PT_i];
    PageTable *PT;
    if(!PDE.GetFlag(PT_Flags::Present)){
        PT = (PageTable*)globalAllocator.RequestPage();
        memset(PT, 0, PAGE_SIZE);
        PDE.SetAddress((uint64_t)PT >> 12);
        PDE.SetFlag(PT_Flags::Present, true);
        PDE.SetFlag(PT_Flags::RW, true);
        PD->entries[indexer.PT_i] = PDE;
    }
    else PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);

    PDE = PT->entries[indexer.P_i];
    PDE.SetAddress((uint64_t)phys >> 12);
    PDE.SetFlag(PT_Flags::Present, true);
    PDE.SetFlag(PT_Flags::RW, true);
    PT->entries[indexer.P_i] = PDE;
}
