#include "lib/system.h"
#include "lib/keyboard.h"

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

void waitForIO(uint32_t timerCount){
    while(1){
        asm volatile("nop");
        timerCount--;
        if(timerCount <= 0)
            break;
    }
}
void sleep(uint32_t timerCount){
    waitForIO(timerCount);
}