#include <drivers/framebuffer.hpp>
#include <kernel.hpp>
#include <drivers/font.hpp>
#include <stdarg.h>
#include <include/string.hpp>
#include <math.hpp>

void setRGB(Color *col, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    col->red = r;
    col->green = g;
    col->blue = b;
    col->alpha = a;
}

Color RGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    Color output;
    output = {r, g, b, a};
    return output;
}

Color HEX2RGB(uint32_t hex){
    Color output;
    output.red = ((hex >> 24) & 0xFF);
    output.green = ((hex >> 16) & 0xFF);
    output.blue = ((hex >> 8) & 0xFF);
    output.alpha = (hex & 0xFF);

    return output;
}

uint32_t GetColor(Color col){
    return (( ((col.red & 0xFF) << 24) + ((col.green & 0xFF) << 16) + ((col.blue & 0xFF) << 8) + (col.alpha & 0xFF) ));
}

void Framebuffer::Initialize(Color scheme[8], stivale2_struct_tag_framebuffer *info, int fontSpecifier){
    for(int i = 0; i < 7; ++i){
        colorScheme[i] = scheme[i];
    }

    monitorSpecs[0] = info->framebuffer_width;
    monitorSpecs[1] = info->framebuffer_height;
    
    thisFB = info;
    cursorX = 0;
    cursorY = 0;
    font = fontSpecifier;
}

void Framebuffer::SetColors(Color fg, Color bg){
    foreground = fg;
    background = bg;
}

//Drawing functions

uint32_t Framebuffer::GetPixel(int x, int y){
    size_t pixel = y * thisFB->framebuffer_width + x;
    uint32_t *fb = (uint32_t *)thisFB->framebuffer_addr;
    
    return (fb[pixel] << 8) + 255;
}

void Framebuffer::DrawPixel(int x, int y, uint32_t color){
    size_t pixel = y * thisFB->framebuffer_width + x;
    uint32_t *fb = (uint32_t *)thisFB->framebuffer_addr;
    uint8_t alpha = (color << 24) >> 24;

    if(alpha == 0){
        return;
    }
    else if(alpha == 255){
        fb[pixel] = color >> 8;
        return;
    }

    fb[pixel] = (GetPixel(x, y) * (alpha << 8 + alpha) + color * ((alpha ^ 0xFF) << 8 + (alpha ^ 0xFF))) >> 16;
}

void Framebuffer::DrawHorizontalLine(long startX, long startY, long dx, Color col){
    for(long i = 0; i < dx; ++i){
        DrawPixel(startX + i, startY, GetColor(col));
    }
}
void Framebuffer::DrawVerticalLine(long startX, long startY, long dy, Color col){
    for(long i = 0; i < dy; ++i){
        DrawPixel(startX, startY + 1, GetColor(col));
    }
}
void Framebuffer::DrawDiagonalLine(long startX, long startY, long endX, long endY, Color col){
    int i;
    int sdx = sign(endX);
    int sdy = sign(endY);
    int dxabs = abs(endX);
    int dyabs = abs(endY);
    int x = dyabs >> 1;
    int y = dxabs >> 1;
    long px = startX;
    long py = startY;

    if (dxabs >= dyabs)
    {
        for (i = 0; i < dxabs; i++)
        {
            y += dyabs;
            if (y >= dxabs)
            {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            DrawPixel(px, py, GetColor(col));
        }
    }
    else
    {
        for (i = 0; i < dyabs; i++)
        {
            x += dxabs;
            if (x >= dyabs)
            {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            DrawPixel(px, py, GetColor(col));
        }
    }
}
void Framebuffer::DrawLine(long startX, long startY, long endX, long endY, Color col){
    /*if(startX > monitor->framebuffer_width) startX = monitor->framebuffer_width - 1;
    if(endX > monitor->framebuffer_width) endX = monitor->framebuffer_width - 1;
    if(startY > monitor->framebuffer_height) startY = monitor->framebuffer_height - 1;
    if(endY > monitor->framebuffer_height) endY = monitor->framebuffer_height - 1;

    if(startX < -1) startX = -1;
    if(endX < -1) endX = -1;
    if(startY < -1) startY = -1;
    if(endY < -1) endY = -1;*/

    long dx = endX - startX, dy = endY - startY;

    if(dy == 0){
        DrawHorizontalLine(startX, startY, dx, col);
    }
    if(dx == 0){
        DrawVerticalLine(startX, startY, dy, col);
    }

    DrawDiagonalLine(startX, startY, dx, dy, col);
}

void Framebuffer::DrawTriangle(Point2 first, Point2 second, Point2 third, Color col){
    DrawLine(first.x, first.y, second.x, second.y, col);
    DrawLine(first.x, first.y, third.x, third.y, col);
    DrawLine(second.x, second.y, third.x, third.y, col);
}
void Framebuffer::DrawTriangleFilled(Point2 first, Point2 second, Point2 third, Color col){
    DrawTriangle(first, second, third, col);

    
}

void Framebuffer::DrawRect(int x, int y, int w, int h, Color col){
    DrawLine(x, y, x + w, y, col);
    DrawLine(x + w, y, x + w, y + h, col);
    DrawLine(x + w, y + h, x, y + h, col);
    DrawLine(x, y + h, x, y, col);
}
void Framebuffer::DrawRectFilled(int x, int y, int w, int h, Color fill){
    for(int i = 0; i < h; ++i){
        DrawLine(x, y + i, x + w, y + i, fill);
    }
}

void Framebuffer::DrawCircle(int cx, int cy, int rad, Color col){
    int x = -rad, y = 0, err = 2 - 2 * rad;

    do{
        DrawPixel(abs(cx - x), abs(cy + y), GetColor(col));
        DrawPixel(abs(cx + x), abs(cy + y), GetColor(col));
        DrawPixel(abs(cx - x), abs(cy - y), GetColor(col));
        DrawPixel(abs(cx + x), abs(cy - y), GetColor(col));
        
        rad = err;
        if (rad > x) err += ++x * 2 + 1;
        if (rad <= y) err += ++y * 2 + 1;
    }while(x < 0);
}
void Framebuffer::DrawCircleFilled(int cx, int cy, int rad, Color col){
    if ((rad > cx) | (rad > cy)) { cx = rad; cy = rad; };
    int x = rad;
    int y = 0;
    int xC = 1 - (rad << 1);
    int yC = 0;
    int err = 0;

    while (x >= y)
    {
        for (uint32_t i = cx - x; i <= cx + x; i++)
        {
            DrawPixel(i, cy + y, GetColor(col));
            DrawPixel(i, cy - y, GetColor(col));
        }
        for (uint32_t i = cx - y; i <= cx + y; i++)
        {
            DrawPixel(i, cy + x, GetColor(col));
            DrawPixel(i, cy - x, GetColor(col));
        }

        y++;
        err += yC;
        yC += 2;
        if (((err << 1) + xC) > 0)
        {
            x--;
            err += xC;
            xC += 2;
        }
    }
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

//Textual functions

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
                PrintS(to_hstring(va_arg(lst, long)));
                break;
        }
        c++;
    }
    va_end(lst);

    for(int i = 0; i < 128; ++i){
        buffer[i] = 0;
    }
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
            case 'u':
                PrintS(itoa(va_arg(lst, uint64_t), buffer, 10));
                break;
            case 'x':
                PrintS(to_hstring(va_arg(lst, long)));
                break;
        }
        c++;
    }
    va_end(lst);

    for(int i = 0; i < 128; ++i){
        buffer[i] = 0;
    }
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
    PrintC('\n');

    SetColors(RGB(255, 255, 255), RGB(0, 0, 0));
}
void Framebuffer::Log(int type, char *c, ...){
    PrintC('[');
    switch (type){
        case SUCCESS:
            SetColors(RGB(0, 255, 0), RGB(0, 0, 0));
            PrintS("SUCCES");
            break;
        case WARNING:
            SetColors(RGB(255, 200, 0), RGB(0, 0, 0));
            PrintS("WARNING");
            break;
        case ERROR:
            SetColors(RGB(255, 0, 0), RGB(0, 0, 0));
            PrintS("ERROR");
            break;
        default:
            break;
    }
    SetColors(RGB(255, 255, 255), RGB(0, 0, 0));
    PrintS("] ");

    va_list list;
    PrintF(c, list);
    PrintC('\n');
}
void Framebuffer::Panic(uint8_t *c, ...){
    Clear();
    va_list lst;
    PrintF(c, lst);
    asm("hlt");
}
//Others

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