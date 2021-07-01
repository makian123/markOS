#ifndef SYSTEM_H
#define SYSTEM_H

#include "types.h"

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);
uint8_t getInput();
uint8_t getInputNoLoop();
void waitForIO(uint32_t timerCount);
void sleep(uint32_t seconds);

#endif