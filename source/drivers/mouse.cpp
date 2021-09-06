#include <drivers/mouse.hpp>
#include <include/io.hpp>
#include <system/idt/idt.hpp>
#include <drivers/framebuffer.hpp>

uint8_t mouseCycle = 0;
char mouseByte[3];
Framebuffer fb;

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
            //else if(mousePos.x < monitor->framebuffer_width - 1) mousePos.x = monitor->framebuffer_width - 1;

            if(mousePos.y < 0) mousePos.y = 0;
            //else if(mousePos.y < monitor->framebuffer_height - 1) mousePos.y = monitor->framebuffer_height - 1;

            mouseCycle = 0;
            break;
    }
    fb.DrawPixel(mousePos.x, mousePos.y, GetColor(RGB(255, 255, 255)));
}

void mouseInit(Framebuffer in){
    fb = in;
    uint8_t status;

    outb(0x64, 0xAB);

    mouseWait(1);
    outb(0x64, 0x20);
    mouseWait(0);
    status=(inb(0x60) | 2);
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