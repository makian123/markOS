#include "mouse.hpp"
#include <include/io.hpp>
#include <system/idt/idt.hpp>
#include "framebuffer.hpp"

#define PS2LeftBtn 0b00000001
#define PS2RightBtn 0b00000010
#define PS2MiddleBtn 0b00000100


uint8_t mouseCycle = 0;
Point2 oldMouse;
char mouseByte[3];
Framebuffer fb;

uint32_t mouseBMP2[] {
    0x00000000, 0x010101FF, 0x010101FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x010101FF, 0x010101FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x010101FF, 0xFFFFFFFF, 0xFFFFFFFF, 0x010101FF, 0x00000000, 0x00000000, 0x010101FF, 0x010101FF, 0xBCBCBCFF, 0xFFFFFFFF, 0x010101FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x010101FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x010101FF, 0x010101FF, 0xBCBCBCFF, 0xFFFFFFFF, 0x010101FF, 0xBCBCBCFF, 0xFFFFFFFF, 0x010101FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x010101FF, 0xBCBCBCFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x010101FF, 0xBCBCBCFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x010101FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x010101FF, 0xBCBCBCFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x7A7A7AFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x010101FF, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x010101FF, 0xBCBCBCFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x7A7A7AFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x7A7A7AFF, 0xFFFFFFFF, 0x010101FF, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x010101FF, 0x010101FF, 0xBCBCBCFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x7A7A7AFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x010101FF, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x010101FF, 0x7A7A7AFF, 0x7A7A7AFF, 0xBCBCBCFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x010101FF, 0x010101FF, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x010101FF, 0x7A7A7AFF, 0xBCBCBCFF, 0xBCBCBCFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x010101FF, 0xFFFFFFFF, 0xFFFFFFFF, 0x010101FF, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x010101FF, 0xBCBCBCFF, 0xBCBCBCFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x010101FF, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x010101FF, 0xBCBCBCFF, 0xBCBCBCFF, 0xBCBCBCFF, 0xBCBCBCFF, 0x010101FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xBCBCBCFF, 0x010101FF, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x010101FF, 0x010101FF, 0x010101FF, 0x010101FF, 0xBCBCBCFF, 0xFFFFFFFF, 0xBCBCBCFF, 0x010101FF, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x010101FF, 0x7A7A7AFF, 0xBCBCBCFF, 0xBCBCBCFF, 0x010101FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x010101FF, 0x7A7A7AFF, 0x7A7A7AFF, 0x010101FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x010101FF, 0x010101FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x000000
    
};

void ClearMouseCursor(Point2 position){
    if(!fb.mouseDrawn) return;

    int maxX = 16, maxY = 16;

    int diffX = fb.monitorSpecs[0] - mousePos.x;
    int diffY = fb.monitorSpecs[1] - mousePos.y;

    if(diffX < 16) maxX = diffX;
    if(diffY < 16) maxY = diffY;

    for(int y = 0; y < maxY; ++y){
        for(int x = 0; x < maxX; ++x){
            if(HEX2RGB(mouseBMP2[x + y * 16]).alpha == 0) continue;
            fb.DrawPixel(position.x + x, position.y + y, GetColor(fb.mouseBuffer[x + y * 16]));
        }
    }
}

void DrawCursor(){
    int maxX = 16, maxY = 16;

    int diffX = fb.monitorSpecs[0] - mousePos.x;
    int diffY = fb.monitorSpecs[1] - mousePos.y;

    if(diffX < 16) maxX = diffX;
    if(diffY < 16) maxY = diffY;
    
    for(int y = 0; y < maxY; ++y){
        for(int x = 0; x < maxX; ++x){
            if(HEX2RGB(mouseBMP2[x + y * 16]).alpha == 0) continue;
            fb.mouseBuffer[x + y * 16] = HEX2RGB(fb.GetPixel(mousePos.x + x, mousePos.y + y));
            fb.DrawPixel(mousePos.x + x, mousePos.y + y, mouseBMP2[x + y * 16]);
        }
    }

    fb.mouseDrawn = true;
}

void mouseWait(uint8_t type){
    uint32_t timeout = 100000;

    if(type == 0){
        while(timeout--){
            if((inb(0x64) & 1) == 1){
                return;
            }
        }
        return;
    }
    else{
        while (timeout--)
        {
            if((inb(0x64) & 2) == 0){
                return;
            }
        }
        return;
    }
}

void mouseWrite(uint8_t data){
    mouseWait(1);
    outb(0x64, 0xD4);
    mouseWait(1);
    outb(0x60, data);
}

uint8_t mouseRead(){
    mouseWait(0);
    return inb(0x60);
}

void mouseHandler(struct interrupt_registers *){
    switch (mouseCycle)
    {
        case 0:
            mouseByte[0] = mouseRead();
            mouseCycle++;
            break;
        case 1:
            mouseByte[1] = mouseRead();
            mouseCycle++;
            break;
        case 2:
            mouseByte[2] = mouseRead();
            mousePos = { mousePos.x + mouseByte[1], mousePos.y - mouseByte[2] };
            if(mousePos.x < 0) mousePos.x = 0;
            else if(mousePos.x >= fb.monitorSpecs[0] - 1) mousePos.x = fb.monitorSpecs[0] - 1;

            if(mousePos.y < 0) mousePos.y = 0;
            else if(mousePos.y >= fb.monitorSpecs[1] - 1) mousePos.y = fb.monitorSpecs[1] - 1;
            
            mouseCycle = 0;
            break;
    }

    ClearMouseCursor(oldMouse);
    
    if(mouseByte[0] & PS2LeftBtn) fb.DrawPixel(mousePos.x, mousePos.y,      0xFF0000FF);
    if(mouseByte[0] & PS2MiddleBtn) fb.DrawPixel(mousePos.x, mousePos.y,    0xFFFF00FF);
    if(mouseByte[0] & PS2RightBtn) fb.DrawPixel(mousePos.x, mousePos.y,     0xFFFFFFFF);

    DrawCursor();

    oldMouse = mousePos;
}

void mouseInit(Framebuffer in){
    fb = in;
    uint8_t status;

    outb(0x64, 0xA8);

    mouseWait(1);
    outb(0x64, 0x20);
    mouseWait(0);
    status = (inb(0x60) | 2);
    mouseWait(1);
    outb(0x64, 0x60);
    mouseWait(1);
    outb(0x60, status);
    
    mouseWrite(0xF6);
    mouseRead();
    
    mouseWrite(0xF4);
    mouseRead();

    registerInterrupt(IRQ12, mouseHandler);
}