#ifndef CHAR_H
#define CHAR_H

#include "types.h"
#include "keyboard.h"


extern uint8_t get_ascii_char(uint8_t c, bool caps);
void putC(uint8_t ch);

#endif