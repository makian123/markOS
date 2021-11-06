#include <stivale2.h>
#include <include/types.hpp>
#include <main.hpp>

#include <drivers/keyboard.hpp>
#include <drivers/mouse.hpp>
#include <drivers/pit.hpp>
#include <system/gdt/gdt.hpp>
#include <system/idt/idt.hpp>
#include <include/io.hpp>

#include <kernel.hpp>
#include <drivers/framebuffer.hpp>
#include <boot/bootVars.hpp>

#include <mm/mem.hpp>
#include <mm/pageFrameAllocator.hpp>
#include <mm/pageTableManager.hpp>
#include <mm/heap.hpp>

struct node{
    int val;
    node *next;
    node *prev;
};

extern "C" uint64_t _KernelStart;
extern "C" uint64_t _KernelEnd;

CRs getCRs()
{
    uint64_t cr0, cr2, cr3;
    asm volatile (
        "mov %%cr0, %%rax\n\t"
        "mov %%eax, %0\n\t"
        "mov %%cr2, %%rax\n\t"
        "mov %%eax, %1\n\t"
        "mov %%cr3, %%rax\n\t"
        "mov %%eax, %2\n\t"
    : "=m" (cr0), "=m" (cr2), "=m" (cr3)
    : /* no input */
    : "%rax"
    );
    return {cr0, cr2, cr3};
}

extern "C" void _start(stivale2_struct *info)
{
    asm volatile("sti");
    stivale2_struct_tag_framebuffer *monitor = (stivale2_struct_tag_framebuffer*)stivale2_get_tag(info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    stivale2_struct_tag_smp *smp = (stivale2_struct_tag_smp*)stivale2_get_tag(info, STIVALE2_STRUCT_TAG_SMP_ID);
    stivale2_struct_tag_memmap *memoryMap = (stivale2_struct_tag_memmap*)stivale2_get_tag(info, STIVALE2_STRUCT_TAG_MEMMAP_ID);
    stivale2_struct_tag_modules *mod = (stivale2_struct_tag_modules*)stivale2_get_tag(info, STIVALE2_STRUCT_TAG_MODULES_ID);

    Color scheme[8] = {
        scheme[0] = RGB(25, 25, 25),
        scheme[1] = RGB(220, 50, 47),
        scheme[2] = RGB(133, 153, 0),
        scheme[3] = RGB(181, 137, 0),
        scheme[4] = RGB(38, 139, 210),
        scheme[5] = RGB(108, 113, 196),
        scheme[6] = RGB(42, 161, 152),
        scheme[7] = RGB(255, 255, 255)
    };

    Framebuffer backBuffer;
    backBuffer.Initialize(scheme, monitor, 16);
    backBuffer.SetColors(scheme[7], scheme[0]);
    backBuffer.Clear();

    initGDT();

    backBuffer.PrintF("CPU cores: %d\n", smp->cpu_count);

    /*Memory*/
    globalAllocator = PageFrameAllocator();
    globalAllocator.Initialize(memoryMap);

    globalAllocator.LockPages((void*)_KernelStart, ((uint64_t)_KernelStart - (uint64_t)_KernelEnd) / 4096 + 1);

    PageTable *PML4 = (PageTable*)getCRs().cr3;
    globalPageTableManager.PML4Addr = PML4;

    for(uint64_t i = 0; i < getMemSize(memoryMap); i += 0x1000){
        //ptManager.MapMemory((void*)i, (void*)i);
    }

    uint64_t fbSize = monitor->framebuffer_bpp * monitor->framebuffer_height * monitor->framebuffer_width + 0x1000;
    globalAllocator.LockPages((void*)monitor->framebuffer_addr, fbSize / 0x1000 + 1);

    for(uint64_t i = monitor->framebuffer_addr; i < monitor->framebuffer_addr + fbSize; i += 4096){
        globalPageTableManager.MapMemory((void*)i, (void*)i);
    }

    asm volatile("mov %0, %%cr3" : : "r" (PML4));

    memset((void*)monitor->framebuffer_addr, 0, monitor->framebuffer_bpp * monitor->framebuffer_height * monitor->framebuffer_width);
    backBuffer.Clear();

    initHeap((void*)0x000010000000, 0x10);

    backBuffer.PrintF("Memory size: %u MB\n", getMemSize(memoryMap) / 1024 / 1024);

    for(uint64_t i = 0; i < memoryMap->entries; ++i){
        stivale2_mmap_entry entry = memoryMap->memmap[i];
        backBuffer.PrintF("Start and end: %x - %x, type: ", entry.base, (entry.base + entry.length));
        switch (entry.type)
        {
            case STIVALE2_MMAP_USABLE:
                backBuffer.PrintF("Usable memory\n");
                break;
            case STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE:
                backBuffer.PrintF("Bootloader reclaimable\n");
                break;
            case STIVALE2_MMAP_KERNEL_AND_MODULES:
                backBuffer.PrintF("Kernel and modules\n");
                break;
            case STIVALE2_MMAP_FRAMEBUFFER:
                backBuffer.PrintF("Framebuffer\n");
                break;
            default:
                backBuffer.PrintF("Default memory\n");
                break;
        }
    }

    /*Interrupts*/
    initIDT(&backBuffer);

    initPIT(100);
    initKeyboard(backBuffer);
    mouseInit(backBuffer);

    //void *tmp = malloc(100);

    //initATA();
    main(info, &backBuffer);
}
