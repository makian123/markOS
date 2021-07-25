#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"

extern void (*stivale2_print)(const char *buf, size_t size);

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);

uint8_t getInput();
void waitForIO(uint32_t timerCount);
void sleep(uint32_t timerCount);
void startInput();

#endif