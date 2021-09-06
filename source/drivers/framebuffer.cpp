#include <drivers/framebuffer.hpp>
#include <kernel.hpp>
#include <drivers/font.hpp>
#include <stdarg.h>
#include <include/string.hpp>

void setRGB(Color *col, uint8_t r, uint8_t g, uint8_t b){
    col->red = r;
    col->green = g;
    col->blue = b;
}

Color RGB(uint8_t r, uint8_t g, uint8_t b){
    Color output;
    output = {r, g, b};
    return output;
}

Color HEX2RGB(uint32_t hex){
    Color output;
    output.red = ((hex >> 16) & 0xFF);
    output.green = ((hex >> 8) & 0xFF);
    output.blue = (hex & 0xFF);

    return output;
}

uint32_t GetColor(Color col){
    return (((col.red & 0xFF) << 16) + ((col.green & 0xFF) << 8) + (col.blue & 0xFF));
}

void Framebuffer::Initialize(Color scheme[8], stivale2_struct_tag_framebuffer *info, int fontSpecifier){
    for(int i = 0; i < 7; ++i){
        colorScheme[i] = scheme[i];
    }
    thisFB = info;
    cursorX = 0;
    cursorY = 0;
    font = fontSpecifier;
}

void Framebuffer::SetColors(Color fg, Color bg){
    foreground = fg;
    background = bg;
}

void Framebuffer::DrawPixel(int x, int y, int color){
    size_t pixel = y * thisFB->framebuffer_width + x;
    uint32_t *fb = (uint32_t *)thisFB->framebuffer_addr;

    fb[pixel] = color;
}

void Framebuffer::DrawLine(long startX, long startY, long endX, long endY, Color col){
    long x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;
    dx= endX - startY;
    dy= endY - startY;
    dx1 = dx < 0 ? -dx : dx;
    dy1 = dy < 0 ? -dy : dy;
    px=2*dy1-dx1;
    py=2*dx1-dy1;
    if(dy1<=dx1)
    {
        if(dx>=0)
            {
            x = startX;
            y = startY;
            xe = endX;
        }
        else
        {
            x = endX;
            y = endY;
            xe = startX;
        }
        DrawPixel(x, y, GetColor(col));
        for(i=0;x<xe;i++)
        {
            x=x+1;
            if(px<0)
            {
                px=px+2*dy1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                y=y+1;
                }
                else
                {
                y=y-1;
                }
                px=px+2*(dy1-dx1);
            }
            DrawPixel(x, y, GetColor(col));
        }
    }
    else
    {
        if(dy>=0)
        {
            x = startX;
            y = y;
            ye = endY;
        }
        else
        {
            x = endX;
            y = endY;
            ye = startY;
        }
        DrawPixel(x, y, GetColor(col));
        for(i=0;y<ye;i++)
        {
            y=y+1;
            if(py<=0)
            {
                py=py+2*dx1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    x=x+1;
                }
                else
                {
                    x=x-1;
                }
                py=py+2*(dx1-dy1);
            }
            DrawPixel(x, y, GetColor(col));
        }
    }
}

void Framebuffer::DrawRect(int x, int y, int w, int h, Color fill){
    w += x;
    h += y;

    for(int tY = y; tY < h; ++tY){
        for(int tX = x; tX < w; ++tX){
            DrawPixel(tX, tY, GetColor(fill));
        }
    }
}

void Framebuffer::PrintC(uint8_t ch){
    Color white = RGB(255, 255, 255);
    if(ch == '\b'){
        if(cursorX == 0 && cursorY == 0) return;
        if(cursorX == 0 && cursorY > 0){
            cursorX = (thisFB->framebuffer_width / 8) - 1;
            cursorY--;
        }
        cursorX--;
        for(int row = 0; row < font; ++row){
            for(int column = 0; column < 8; ++column){
                DrawPixel(column + (cursorX * 8), row + (cursorY * font), GetColor(background));
            }
        
        }
        return;
    }
    else if(ch == '\n'){
        cursorY++;
        cursorX = 0;
        return;
    }

    if(font == 8){
        for(int row = 0; row < 8; ++row){
            for(int column = 0; column < 8; ++column){
                if(font8[ch][row] & (1 << column)){
                    DrawPixel(column + (cursorX * 8), row + (cursorY * 8), GetColor(foreground));
                }
            }
        }
    }
    else if(font == 16){
        for(int row = 0; row < 16; ++row){
            for(int column = 0; column < 8; ++column){
                if(font16[ch][row] & (1 << column)){
                    DrawPixel(7 - column + (cursorX * 8), row + (cursorY * 16), GetColor(foreground));
                }
            }
        }
    }

    cursorX++;
    if(cursorX >= (thisFB->framebuffer_width / 8) - 1){
        cursorX = 0;
        cursorY++;
    }
}

void Framebuffer::PrintC(char input){
    PrintC((uint8_t)input);
}

void Framebuffer::PrintS(const uint8_t *input){
    while(*input != '\0'){
        PrintC(*input);
        input++;
    }
}

void Framebuffer::PrintS(const char *input){
    while(*input != '\0'){
        PrintC(*input);
        input++;
    }
}

void Framebuffer::PrintF(uint8_t *c, ...){
    uint8_t buffer[128];
    va_list lst;
    va_start(lst, c);
    while(*c != '\0'){
        if(*c != '%'){
            PrintC(*c);
            c++;
            continue;
        }
        c++;
        switch(*c){
            case 'c':
                PrintC((uint8_t)va_arg(lst, int));
                break;
            case 's':
                PrintS(va_arg(lst, const uint8_t*));
                break;
            case 'd':
            case 'i':
            case 'l':
                PrintS(itoa(va_arg(lst, long), buffer, 10));
                break;
            case 'x':
                PrintS("0x");
                PrintS(itoa(va_arg(lst, long), buffer, 10));
                break;
        }
        c++;
    }
    va_end(lst);
}

void Framebuffer::PrintF(char *c, ...){
    uint8_t buffer[128];
    va_list lst;
    va_start(lst, c);
    while(*c != '\0'){
        if(*c != '%'){
            PrintC(*c);
            c++;
            continue;
        }
        c++;
        switch(*c){
            case 'c':
                PrintC((char)va_arg(lst, int));
                break;
            case 's':
                PrintS(va_arg(lst, const uint8_t*));
                break;
            case 'd':
            case 'i':
            case 'l':
                PrintS(itoa(va_arg(lst, long), buffer, 10));
                break;
            case 'x':
                PrintS("0x");
                PrintS(itoa(va_arg(lst, long), buffer, 10));
                break;
        }
        c++;
    }
    va_end(lst);
}

void Framebuffer::Log(int type, uint8_t *c, ...){
    switch (type){
        case SUCCESS:
            SetColors(RGB(0, 255, 0), RGB(0, 0, 0));
            break;
        case WARNING:
            SetColors(RGB(255, 200, 0), RGB(0, 0, 0));
            break;
        case ERROR:
            SetColors(RGB(255, 0, 0), RGB(0, 0, 0));
            break;
        default:
            break;
    }

    va_list list;
    PrintF(c, list);
    va_end(list);
    PrintC('\n');

    SetColors(RGB(255, 255, 255), RGB(0, 0, 0));
}

void Framebuffer::Log(int type, char *c, ...){
    switch (type){
        case SUCCESS:
            SetColors(RGB(0, 255, 0), RGB(0, 0, 0));
            break;
        case WARNING:
            SetColors(RGB(255, 200, 0), RGB(0, 0, 0));
            break;
        case ERROR:
            SetColors(RGB(255, 0, 0), RGB(0, 0, 0));
            break;
        default:
            break;
    }

    va_list list;
    PrintF(c, list);
    va_end(list);
    PrintC('\n');

    SetColors(RGB(255, 255, 255), RGB(0, 0, 0));
}

void Framebuffer::PrintBMP(int xPos, int yPos, BITMAP bmp){
    int width = bmp.header[18];
    int height = bmp.header[22];
    long count = width * height * 3;
    uint32_t hexCode = 0;
    for(int y = yPos; y < yPos + height; ++y){
        for(int x = xPos; x < xPos + width; ++x){
            hexCode = bmp.data[count--];
            hexCode = (hexCode << 8) | bmp.data[count--];
            hexCode = (hexCode << 8) | bmp.data[count--];
            
            DrawPixel(x, y, GetColor(HEX2RGB(hexCode)));
        }
    }
}

void Framebuffer::CursorMove(uint8_t ch){
    switch (ch)
    {
        case 'u':
            cursorY -= (cursorY == 0 ? 0 : 1);
            break;
        case 'd':
            cursorY++;
            break;
        case 'l':
            cursorX -= (cursorX == 0 ? 0 : 1);
            break;
        case 'r':
            cursorX++;
            break;
    }
}

void Framebuffer::Clear(){
    cursorX = 0;
    cursorY = 0;

    for(size_t y = 0; y < thisFB->framebuffer_height * 2; ++y){
        for(size_t x = 0; x < thisFB->framebuffer_width; ++x){
            DrawPixel(x, y, GetColor(background));
        }
    }
}