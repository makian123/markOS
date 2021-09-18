#include <stivale2.h>
#include <include/types.hpp>
#include <main.hpp>
#include <drivers/keyboard.hpp>
#include <drivers/pit.hpp>
#include <system/gdt/gdt.hpp>
#include <system/idt/idt.hpp>
#include <include/io.hpp>
#include <stivale2.h>
#include <kernel.hpp>
#include <drivers/framebuffer.hpp>
#include <drivers/mouse.hpp>
#include <game/game.hpp>
#include <drivers/ata.hpp>
#include <boot/bootVars.hpp>

static uint8_t stack[4096];

static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = 0
    },
    .flags = 0
};
static struct stivale2_header_tag_smp smp_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_SMP_ID,
        .next = (uint64_t)&terminal_hdr_tag
    },
    .flags = 0
};
static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = (uint64_t)&smp_hdr_tag
    },
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};
__attribute__((section(".stivale2hdr"), used))
    static struct stivale2_header stivale_hdr = {
        .entry_point = 0,
        .stack = (uintptr_t)stack + sizeof(stack),
        .flags = (1 << 1),
        .tags = (uintptr_t)&framebuffer_hdr_tag
};
void* stivale2_get_tag(stivale2_struct *stivale, uint64_t id)
{
	stivale2_tag *current_tag = (stivale2_tag*)stivale->tags;
	for (;;) {
		if (current_tag == null) {
			return null;
		}

		if (current_tag->identifier == id) {
			return current_tag;
		}

		current_tag = (stivale2_tag*)current_tag->next;
	}
}

extern "C" void InitSSE();

extern "C" void _start(stivale2_struct *info)
{
    boot_info_t bootVars;

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

    initGDT();

    stivale2_struct_tag_framebuffer *monitor = (stivale2_struct_tag_framebuffer*)stivale2_get_tag(info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    stivale2_struct_tag_smp *smp = (stivale2_struct_tag_smp*)stivale2_get_tag(info, STIVALE2_STRUCT_TAG_SMP_ID);
    stivale2_struct_tag_memmap *memoryMap = (stivale2_struct_tag_memmap*)stivale2_get_tag(info, STIVALE2_STRUCT_TAG_MEMMAP_ID);

    if(smp != NULL){
        bootVars.cpu.processor_count = smp->cpu_count;
        bootVars.cpu.bootstrap_processor_lapic_id = smp->bsp_lapic_id;
        bootVars.cpu.acpi_processor_uid = smp->smp_info->processor_id;
        bootVars.cpu.lapic_id = smp->smp_info->lapic_id;
    }
    if(memoryMap != NULL){
        bootVars.mmap.entries = memoryMap->entries;
        bootVars.mmap.length = memoryMap->memmap->length;
        bootVars.mmap.type = memoryMap->memmap->type;

        for(int i = 0; i < memoryMap->entries; ++i){
            bootVars.mmap.memmap[i] = memoryMap->memmap[i];
            stivale2_mmap_entry *internal = &memoryMap->memmap[i];
        }
    }

    Framebuffer backBuffer;
    backBuffer.Initialize(scheme, monitor, 16);
    backBuffer.SetColors(scheme[7], scheme[0]);
    backBuffer.Clear();

    initIDT();

    initPIT(100);
    initKeyboard(backBuffer);
    mouseInit(backBuffer);

    //initATA();
    main(info, &backBuffer);
}
