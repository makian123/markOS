#include "lib/system.h"

uint32_t ticks = 0;

uint8_t inb(uint16_t port){
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
    return ret;
}
void outb(uint16_t port, uint8_t data){
    asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
}

uint8_t getInput(){
    char ch = 0;
    while((ch = inb(KEYBOARD_PORT)) != 0){
        if(ch > 0)
            return ch;
        }
    return ch;
}

uint8_t getInputNoLoop(){
    char ch = 0;
    ch = inb(KEYBOARD_PORT);
    return ch;
}

void waitForIO(uint32_t timerCount){
    while(1){
        asm volatile("nop");
        timerCount--;
        if(timerCount <= 0)
            break;
    }
}

void sleep(uint32_t sec){
    uint32_t timerTicks;
    timerTicks = ticks + (sec * 25000000);
    while(ticks < timerTicks){
        ticks = ticks + 1;
    }
    ticks = 0;
}

void sleepms(uint32_t ms){
    uint32_t timerTicks;
    timerTicks = ticks + (ms * 25000);
    while(ticks < timerTicks){
        ticks = ticks + 1;
    }
    ticks = 0;
}