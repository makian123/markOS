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
#include "lib/lists.h"
#include "lib/terminal.h"
#include "lib/lists.h"
#include <stdarg.h>

#pragma region commands
uint8_t *cursorUp = "\033[1A\0";
uint8_t *cursorDown = "\033[1B\0";
uint8_t *cursorLeft = "\033[1D\0";
uint8_t *cursorRight = "\033[1C\0";
uint8_t *cursorNextLine = "\033[1E\0";
uint8_t *cursorPrevLine = "\033[1F\0";
uint8_t *moveCursorUL = "\033[H\0";
uint8_t *clearCursorEnd = "\033[0J\0";
uint8_t *clearBeginCursor = "\033[1J\0";
uint8_t *cls = "\033[2J\0";
uint8_t *clearLineCursorEnd = "\033[0K\0";
uint8_t *clearLineCursorBegin = "\033[1K";
uint8_t *clearLine = "\033[2K\0";
uint8_t *pageUp = "\033[S\0";
uint8_t *pageDown = "\033[T\0";
uint8_t *resetColor = "\033[m\0";
uint8_t *bold = "\033[1m\0";

//Foreground colors
uint8_t *fBlack = "\033[30m\0";
uint8_t *fRed = "\033[31m\0";
uint8_t *fGreen = "\033[32m\0";
uint8_t *fYellow = "\033[33m\0";
uint8_t *fBlue = "\033[34m\0";
uint8_t *fMagenta = "\033[35m\0";
uint8_t *fCyan = "\033[36m\0";
uint8_t *fWhite = "\033[37m\0";

//Background colors
uint8_t *bBlack = "\033[40m\0";
uint8_t *bRed = "\033[41m\0";
uint8_t *bGreen = "\033[42m\0";
uint8_t *bYellow = "\033[43m\0";
uint8_t *bBlue = "\033[44m\0";
uint8_t *bMagenta = "\033[45m\0";
uint8_t *bCyan = "\033[46m\0";
uint8_t *bWhite = "\033[47m\0";

#pragma endregion

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
int printS(const uint8_t *str){
    uint32_t i = 0;
    int done = 1;
    while(i < StrLen(str)){
        printC(str[i]);
        i++;
    }
    return done;
}
int printF(const uint8_t *format, ...){
    va_list arg;
    int done;
    va_start(arg, format);
    while(*format != '\0'){
        if(*format != '%'){
            printC(*format);
            format++;
            continue;
        }
        format++;
        if(format == '\0'){
            break;
        }
        switch(*format){
            case 's':
                printS(va_arg(arg, uint8_t*));
                break;
            case 'c':
                printC(va_arg(arg, int));
                break;
            case 'd':
                printS(itoa(va_arg(arg, int), 10));
                break;
            case 'i':
                printS(itoa(va_arg(arg, int), 10));
                break;
            case 'u':
                printS(itoa(va_arg(arg, uint32_t), 10));
                break;
            case 'p':
                printS("0x\0");
                printS(itoa(va_arg(arg, uint32_t), 16));
                break;
            case 'f':
                printS(ftoa(va_arg(arg, float), 2));
            case '%':
                printC('%');
                break;
        }
        format++;
    }
    va_end(arg);
    return done;
}
void printList(struct List *head){
    printC('[');
    while(ListNext(head) != null){
        printS(itoa(ListGetValue(head), 10));
        printS(", \0");
    }
    printC('\b');
    printC(']');
    printC('\n');
}

void RunCommand(const uint8_t *command);
void SetColor(const uint8_t *fg, const uint8_t *bg){
    printS(fg);
    printS(bg);
}
//kernel entry
void kmain(struct stivale2_struct *info) {
    TermInit();
    struct stivale2_tag *tag = (struct stivale2_tag*) info->tags;
    while(tag != null){
        if(tag->identifier == STIVALE2_STRUCT_TAG_TERMINAL_ID){
            struct stivale2_struct_tag_terminal *t = (void*) tag;
            stivale2_print = (void*)(uintptr_t)t->term_write;
        }
        tag = (void*)tag->next;
    }
    
    //InputSystem
    bool caps = false;
    uint8_t inChar;

    for (;;) {
        inChar = getInput();
        if(inChar != 0){
            switch (inChar)
            {
            case KEY_CAPS:
                caps = !caps;
                break;

            case KEY_ENTER:
                if(StrLen(GetCommand()) > 0){
                    RunCommand(GetCommand());
                    CommandClear();
                }
                printS(cursorNextLine);
                break;

            case KEY_UP:
                printS(cursorUp);
                break;
            case KEY_DOWN:
                printS(cursorNextLine);
                break;
            case KEY_LEFT:
                printS(cursorLeft);
                break;
            case KEY_RIGHT:
                printS(cursorRight);
                break;

            case KEY_BACKSPACE:
                printS("\b \b");
                CommandPopBack();
                break;

            case KEY_BACKSLASH:
                printC('\\');
                break;

            default:
                printC(get_ascii_char(inChar, caps));
                CommandAdd(get_ascii_char(inChar, caps));
                break;
            }
            
            inChar = 0;
            sleep(1);
        }
    }
}

void RunCommand(const uint8_t *command){
    if(StrCmp(command, "clear\0")){
        printS(cls);
        printS(moveCursorUL);
    }
    if(StrCmp(command, "color\0")){
        uint8_t fg = '\0', bg = '\0';
        uint8_t *fgName, *bgName;

        printS("\nSet fg color > \0");
        while(true){
            fg = getInput();
            if(fg == KEY_ENTER){
                break;
            }
            else if(fg == KEY_BACKSPACE){
                fg = '\0';
                printC('\b');
            }
            else{
                printC(fg);
            }
        }
        printS("\nSet bg color >\0");
        while(true){
            bg = getInput();
            if(bg == KEY_ENTER){
                break;
            }
            else if(bg == KEY_BACKSPACE){
                bg = '\0';
                printC('\b');
            }
            else{
                printC(bg);
            }
        }

        switch (fg)
        {
        case KEY_0:
            fgName = fBlack;
            break;
        case KEY_1:
            fgName = fRed;
            break;
        case KEY_2:
            fgName = fGreen;
            break;
        case KEY_3:
            fgName = fYellow;
            break;
        case KEY_4:
            fgName = fBlue;
            break;
        case KEY_5:
            fgName = fMagenta;
            break;
        case KEY_6:
            fgName = fCyan;
            break;
        
        default:
            fgName = fWhite;
            break;
        }
        switch (bg)
        {
        case KEY_1:
            bgName = bRed;
            break;
        case KEY_2:
            bgName = bGreen;
            break;
        case KEY_3:
            bgName = bYellow;
            break;
        case KEY_4:
            bgName = bBlue;
            break;
        case KEY_5:
            bgName = bMagenta;
            break;
        case KEY_6:
            bgName = bCyan;
            break;
        case KEY_7:
            bgName = bBlack;
            break;
        default:
            bgName = fWhite;
            break;
        }
    
        SetColor(fgName, bgName);
    }
}