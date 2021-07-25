#include "lib/stivale2.h"
#include "lib/types.h"
#include "lib/conversion.h"
#include "lib/keyboard.h"
#include "lib/kernel.h"
#include "lib/char.h"
#include "lib/string.h"
#include "lib/malloc.h"
#include "lib/system.h"
#include "lib/osMath.h"

extern void __stack_chk_fail(void){
}
void (*stivale2_print)(const char *buf, size_t size) = null;

static uint8_t stack[4096];

static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = 0
    },
    .flags = 0
};

static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = (uint64_t)&terminal_hdr_tag
    },

    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0
};

__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = (1 << 1) | (1 << 2),
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
        current_tag = (void *)current_tag->next;
    }
}

//text printing
void printC(const uint8_t c){
    if (stivale2_print != null)
        stivale2_print(&c, 1);
    asm volatile ("outb %0, %1" :: "a" (c), "Nd" (0xe9) : "memory");
}
void printS(const uint8_t *str){
    uint32_t i = 0;
    while(i < StrLen(str)){
        printC(str[i]);
        i++;
    }
}
void printF(const uint8_t *str){
    uint32_t i = 0;
    while(i < StrLen(str)){
        switch(str[i]){
            case '\n':
                printS("(Newline)\0");
                break;
            default:
                printC(str[i]);
                break;
        }
        i++;
    }
}

void kmain(struct stivale2_struct *info) {
    struct stivale2_tag *tag = (struct stivale2_tag*) info->tags;
    while(tag != null){
        if(tag->identifier == STIVALE2_STRUCT_TAG_TERMINAL_ID){
            struct stivale2_struct_tag_terminal *t = (void*) tag;
            stivale2_print = (void*)(uintptr_t)t->term_write;
        }
        tag = (void*)tag->next;
    }

    printC('a');
    uint8_t inChar;
    
    for (;;) {
        inChar = getInput();
        if(inChar != 0){
            printC(get_ascii_char(inChar, false));
            inChar = 0;
            sleep(1);
        }
    }
}