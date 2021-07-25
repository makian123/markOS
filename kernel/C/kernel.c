#include "lib/stivale2.h"
#include "lib/terminal.h"
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

void kmain(struct stivale2_struct *stivale2_struct) {
    struct stivale2_struct_tag_terminal *term_str_tag;
    term_str_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);

    if (term_str_tag == null) {
        
        for (;;) {
            asm ("hlt");
        }
    }
    
    void *term_write_ptr = (void *)term_str_tag->term_write;
    void (*term_write)(uint8_t *string, size_t length) = term_write_ptr;

    term_write("Welcome to markOS", 17);

    uint8_t inChar;
    uint8_t *tempChar = "0\0";
    
    for (;;) {
        inChar = getInput();
        if(inChar != 0){
            tempChar[0] = get_ascii_char(inChar, false);
            term_write(tempChar, 2);
            inChar = 0;
        }
        waitForIO(0x02FFFFFF);
    }
}