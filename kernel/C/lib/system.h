#ifndef SYSTEM_H
#define SYSTEM_H

#include "types.h"

#define KEYBOARD_PORT 0x60

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);
uint8_t getInput();
uint8_t getInputNoLoop();
void waitForIO(uint32_t timerCount);
void sleep(uint32_t sec);
void sleepms(uint32_t ms);

static inline void sti(){
    asm volatile("sti");
}

static inline void cli(){
    asm volatile("cli");
}

static inline void hang(){
    for(;;);
}

static inline void reboot(){
    for(;;){
        uint32_t i;
        for(i = 0; i < 0x10000; ++i){
            if((inb(0x64) & 0x02) == 0){
                break;
            }
            outb(0x64, 0xFE);
        }
    }
}

//Waits for input/output
static inline void io_wait(void)
{
    outb(0x80, 0x00);
}

static inline uint64_t rCR0() {
    uint64_t ret;
    asm volatile("mov %%cr0, %0" : "=r"(ret));
    return ret;
}

static inline uint64_t rCR2() {
    uint64_t ret;
    asm volatile("mov %%cr2, %0" : "=r"(ret));
    return ret;
}

static inline uint64_t rCR3() {
    uint64_t ret;
    asm volatile("mov %%cr3, %0" : "=r"(ret));
    return ret;
}

static inline uint64_t rCR4() {
    uint64_t ret;
    asm volatile("mov %%cr4, %0" : "=r"(ret));
    return ret;
}

static inline uint64_t rCR8() {
   uint64_t ret;
    asm volatile("mov %%cr8, %0" : "=r"(ret));
    return ret;
}

static inline void wCR0(uint64_t val) {
    asm volatile("mov %0, %%cr0" :: "r"(val));
}

static inline void wCR3(uint64_t val) {
    asm volatile("mov %0, %%cr3" :: "r"(val));
}

static inline void wCR4(uint64_t val) {
    asm volatile("mov %0, %%cr4" :: "r"(val));
}

static inline void wCR8(uint64_t val) {
    asm volatile("mov %0, %%cr8" :: "r"(val));
}

#endif