#include "lib/stivale2.h"
#include "lib/types.h"
#include "lib/conversion.h"
#include "lib/kernel.h"
#include "lib/string.h"
#include "lib/malloc.h"
#include "lib/system.h"
#include "lib/osMath.h"
#include "lib/lists.h"
#include "lib/main.h"
#include <stdarg.h>
#include "lib/framebuffer.h"
#include "lib/gdt.h"
#include "lib/idt.h"

extern void __stack_chk_fail(void){
}

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

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id) {
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
    for (;;) {
        if (current_tag == null) {
            return null;
        }
        if (current_tag->identifier == id) {
            return current_tag;
        }
        current_tag = (struct stivale2_tag*)current_tag->next;
    }
}
void RunCommand(const uint8_t *command);


//kernel entry
void kmain(struct stivale2_struct *info) {
    Color colorscheme[8] = {
        rgb(88, 110, 117),  /* Black */
        rgb(220, 50, 47),   /* Red */
        rgb(133, 153, 0),   /* Green */
        rgb(181, 137, 0),   /* Yellow */
        rgb(38, 139, 210),  /* Blue */
        rgb(108, 113, 196), /* Magenta */
        rgb(42, 161, 152),  /* Cyan */
        rgb(253, 246, 227)  /* White */
    };
    bool good = GDTInit();
    uint8_t *fb_addr = stivale2_get_tag(info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    Framebuffer self = FBInit(colorscheme, info);
    FBClear();
    if(good) glog(SUCCESS, "Successfully loaded GDT\n");
    else{
        glog(FAILURE, "GDT wasn't loaded\n");
        hang();
    }
    main(info, &self);
}