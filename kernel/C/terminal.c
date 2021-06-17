#include "lib/terminal.h"

size_t terminalY, terminalX;
uint8_t terminalColor;
uint16_t* terminalBuffer;
const uint8_t* clsCommand = "Clear";


static inline uint16_t vgaEnter(uint8_t c, uint8_t color){
    return (uint16_t) c | (uint16_t) color << 8;
}

void newLine(){
    terminalBuffer[terminalY * TERM_WIDTH + terminalX] = vgaEnter(' ', terminalColor);
    terminalY++;
    if(terminalY == TERM_HEIGHT) terminalY = 0;
    terminalX = 0;
    clearCurrentLine();
}

void printChar(uint8_t c, bool userIn, uint8_t color){
    //if(userIn) addToCommand(c);
    const size_t index = terminalY * TERM_WIDTH + terminalX;
    terminalBuffer[index] = vgaEnter(c, color);

    terminalX++;
    if(terminalX == TERM_WIDTH){
        terminalX = 0;
        terminalY++;
        if(terminalY == TERM_HEIGHT) terminalY = 0;
    }
    cursorUpdate();

}
void backspace(){
    size_t index = terminalY * TERM_WIDTH + terminalX;
    terminalBuffer[index] = vgaEnter(' ', terminalColor);
    if(terminalX > 1){
        terminalX--;
    }

    index = terminalY * TERM_WIDTH + terminalX;
    terminalBuffer[index] = vgaEnter(' ', terminalColor);
    cursorUpdate();
}

void clearCurrentLine(){
    for(size_t x = 0; x < TERM_WIDTH; ++x){
        const size_t index = terminalY * TERM_WIDTH + x;

        terminalBuffer[index] = vgaEnter(' ', terminalColor);
    }
}
void termClear(){
    for(size_t y = 0; y < TERM_HEIGHT; ++y){
        for(size_t x = 0; x < TERM_WIDTH; ++x){
            const size_t index = y * TERM_WIDTH + x;

            terminalBuffer[index] = vgaEnter(' ', terminalColor);
        }
    }
    terminalY = 0;
    terminalX = 0;
}

void newInputLine(){
    newLine();
    printChar('>', false, terminalColor);
}

void printString(struct string *str){
    while(str->next != null){
        if(str->data == '\n') newLine();
        else printChar(str->data, false, terminalColor);
        str = str->next;
    }
}

void termInit(){
    terminalX = 0;
    terminalY = 0;
    terminalColor = termColor(COLOR_GREEN, COLOR_BLACK);
    terminalBuffer = (uint16_t*) 0xB8000;
    cursorUpdate();
}

void cursorUpdate(){
    terminalBuffer[terminalY * TERM_WIDTH + terminalX] = vgaEnter('_', terminalColor);
}
void clearScreen(){
    for(size_t y = 0; y < TERM_HEIGHT; ++y){
        for(size_t x = 0; x < TERM_WIDTH; ++x){
            terminalBuffer[y * TERM_WIDTH + x] = vgaEnter(' ', terminalColor);
        }
    }

    terminalX = 0;
    terminalY = 0;
    printChar('>', false, terminalColor);
}