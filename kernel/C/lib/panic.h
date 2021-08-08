#ifndef PANIC_H
#define PANIC_H

#include "types.h"

void panic(uint8_t *file, const uint8_t function[20], size_t line, uint8_t *msg);

#endif