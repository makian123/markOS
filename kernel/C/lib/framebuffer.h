#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "types.h"
#include "stivale2.h"
#include <stdarg.h>
#include "psf.h"

typedef struct{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}Color;

Color rgb(int r, int g, int b);

typedef struct{
    Color colorScheme[8];
    Color background;
    Color foreground;

    struct stivale2_struct_tag_framebuffer *fbInfo;
    int cursorX, cursorY;

    PSF font;
}Framebuffer;

Framebuffer FBInit(Color colorScheme[8], struct stivale2_struct *info);
void FBClear();
void PrintC(char c, int position_x, int position_y, Framebuffer *self);
void PrintS(uint8_t *str);
void PrintF(uint8_t *format, ...);
void FBClear();
void glog(int level, uint8_t *format, ...);

void DrawScreen();

enum glog_level
{
    SUCCESS,
    FAILURE,
    SILENT

};

#endif