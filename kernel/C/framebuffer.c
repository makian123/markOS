#include "lib/framebuffer.h"
#include "lib/malloc.h"
#include "lib/kernel.h"
#include "lib/string.h"
#include "lib/font.h"
#include "lib/lists.h"

extern PSF fb_font;

typedef enum
{
    OFF,
    PRE_PARSING,
    PARSING,

} ParsingState;

ParsingState state = OFF;
Framebuffer currentFB;

size_t ptr = 0;
int cursorX = 0, cursorY = 0;
uint8_t nbr[3];

Color rgb(int r, int g, int b){
    return (Color){r, g, b};
}

uint32_t GetColor(Color *col){
    return (((col->red & 0xFF) << 16) + ((col->green & 0xFF) << 8) + (col->blue & 0xFF));
}

void FBDrawPixel(int x, int y, uint32_t col, Framebuffer *self){
    size_t fbI = x + (self->fbInfo->framebuffer_pitch / sizeof(uint32_t)) * y;
    uint32_t *fb = (uint32_t *)self->fbInfo->framebuffer_addr;

    fb[fbI] = col;
}

void FBSet(Framebuffer *self){
    currentFB = *self;
}

void PrintC(char c, int position_x, int position_y, Framebuffer *self){
    switch (c)
    {
        case '\n':
            cursorY++;
            cursorX = 1;
            currentFB.cursorX = cursorX;
            currentFB.cursorY = cursorY;
            return;
        case '\t':
            cursorX += 4;
            currentFB.cursorX = cursorX;
            return;
        case '\033':
            state = PRE_PARSING;
            return;
    }

    if(state == PARSING){
        if(c != ';' && c != 'm'){
            nbr[ptr++] = c;
            return;
        }
    }
    if(state == PRE_PARSING && c == '['){
        state = PARSING;
        return;
    }
    if (state == PARSING && c == 'm')
    {
        int digit = atoi(nbr);
        state = OFF;

        if (digit == 0)
        {
            self->foreground = rgb(131, 148, 150);
            self->background = rgb(0, 43, 54);
        }
        if (digit > 29 && digit < 38)
        {
            self->foreground = self->colorScheme[digit - 30];
        }

        ptr = 0;
        memset(nbr, 0, 3);

        return;
    }

    uint8_t *gylph = &fb_font.data[c * self->font.glyph_size];
    size_t x = position_x * self->font.width, y = position_y * self->font.height;
    static uint8_t masks[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    
    for(size_t i = 0; i < self->font.height; ++i){
        for(size_t j = 0; j < self->font.width; ++j){
            if(gylph[i] & masks[j]){
                FBDrawPixel(x + j, y + i, GetColor(&self->foreground), self);
            }
            else{
                FBDrawPixel(x + j, y + i, GetColor(&self->background), self);
            }
        }
    }

    if(c != '\n'){
        self->cursorX++;
        cursorX++;
    }
}

void PrintS(uint8_t *str){
    currentFB.cursorX = cursorX;
    currentFB.cursorY = cursorY;

    while(*str){
        PrintC(*str++, currentFB.cursorX, currentFB.cursorY, &currentFB);
    }
}

void PrintF(uint8_t *format, ...){
    va_list arg;
    va_start(arg, format);

    while(*format != '\0'){
        if(*format != '%'){
            PrintC(*format, cursorX, cursorY, &currentFB);
            format++;
            continue;
        }
        format++;
        switch(*format){
            case 'd':
            case 'i':
            case 'l':
                PrintS(itoa(va_arg(arg, long), 10));
                break;
            case 'x':
                PrintS(itoa(va_arg(arg, long), 16));
                break;
            case 'c':
                PrintC(va_arg(arg, int), currentFB.cursorX, currentFB.cursorY, &currentFB);
                break;
            case 's':
                PrintS(va_arg(arg, uint8_t*));
                break;
        }
        format++;
    }

    va_end(arg);
}

void FBClear(){
    cursorX = 1;
    cursorY = 0;
    currentFB.cursorX = 1;
    currentFB.cursorY = 0;
    for (size_t x = 0; x < currentFB.fbInfo->framebuffer_width; x++)
    {
        for (size_t y = 0; y < currentFB.fbInfo->framebuffer_height; y++)
        {
            FBDrawPixel(x, y, GetColor(&currentFB.background), &currentFB);
        }
    }
}

void glog(int level, uint8_t *format, ...){
    va_list ap;
    va_start(ap, format);

    switch(level){
        case SUCCESS:
            currentFB.foreground = rgb(0, 255, 0);
            break;
        case SILENT:
            break;
        case FAILURE:
            currentFB.foreground = rgb(255, 0, 0);
            break;
    }
    PrintF(format, ap);
    currentFB.foreground = rgb(255, 255, 255);
    va_end(ap);
    PrintS("\n");
}


Framebuffer FBInit(Color cs[8], struct stivale2_struct *info){
    Framebuffer new;

    new.fbInfo = stivale2_get_tag(info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    for(int i = 0; i < 7; ++i){
        new.colorScheme[i] = cs[i];
    }

    new.foreground = rgb(255, 255, 255);
    new.background = rgb(19, 19, 19);
    new.cursorX = 1;
    new.cursorY = 0;
    new.font = fb_font;
    currentFB = new;
    return new;
}