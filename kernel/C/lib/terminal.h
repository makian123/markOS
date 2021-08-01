#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"

void CommandAdd(uint8_t ch);
void CommandPopBack();
void TermInit();

void CommandClear();
uint8_t *GetCommand();
#endif