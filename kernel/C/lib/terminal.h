#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"
#include "string.h"

#define REG_CTR 0x3d4

enum terminal_colors{
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_AQUA = 3,
    COLOR_RED = 4,
    COLOR_PURPLE = 5,
    COLOR_YELLOW = 6,
    COLOR_WHITE = 7,
    COLOR_GRAY = 8,
    COLOR_LBLUE = 9,
    COLOR_LGREEN = 10,
    COLOR_LAQUA = 11,
    COLOR_LRED = 12,
    COLOR_LPURPLE = 13,
    COLOR_LYELLOW = 14,
    COLOR_LWHITE = 15,
};

static const uint16_t TERM_WIDTH = 80;
static const uint16_t TERM_HEIGHT = 25;

static inline uint8_t termColor(enum terminal_colors fg, enum terminal_colors bg){
    return fg | bg << 4;
}
static inline uint16_t vgaEnter(uint8_t c, uint8_t color);
void newLine();
void printChar(uint8_t c, bool userIn, uint8_t color);
void backspace();
void clearCurrentLine();
void termClear();
void newInputLine();
void termInit();
void cursorUpdate();
void addInputChar(uint8_t c);
void printString(uint8_t *str);

/*Commands*/
void ClearScreen();

void CommandAdd(uint8_t c);
void ExecuteCommand();
void CommandClear();
void CommandPopBack();

#endif