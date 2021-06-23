#include "lib/terminal.h"

size_t terminalY, terminalX;
uint8_t terminalColor;
uint16_t* terminalBuffer;
uint8_t *command;

/*Commands*/
uint8_t *CLS = "Clear\0";
uint8_t *QUIT = "Quit\0";

extern void shutdown();

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

void printString(uint8_t *str){
    size_t i = 0;
    while(str[i] != '\0'){
        if(str[i] == '\n') newLine();
        else printChar(str[i], false, terminalColor);
        ++i;
    }
}

void termInit(){
    command = "ads\0";
    StrClear(command);

    terminalX = 0;
    terminalY = 0;
    terminalColor = termColor(COLOR_GREEN, COLOR_BLACK);
    terminalBuffer = (uint16_t*) 0xB8000;
    cursorUpdate();
}

void cursorUpdate(){
    terminalBuffer[terminalY * TERM_WIDTH + terminalX] = vgaEnter('_', terminalColor);
}

void ClearScreen(){
    for(size_t y = 0; y < TERM_HEIGHT; ++y){
        for(size_t x = 0; x < TERM_WIDTH; ++x){
            terminalBuffer[x * TERM_WIDTH + y] = vgaEnter(' ', terminalColor);
        }
    }

    terminalX = 0;
    terminalY = 0;
    printChar('>', false, terminalColor);
}

void CommandAdd(uint8_t ch){
    StringAdd(command, ch);
}

void CommandClear(){
    StrClear(command);
}

void CommandPopBack(){
    if(StrLen(command) > 0) StrPopBack(command);
}

void ExecuteCommand(){
    if(StrCmp(command, CLS)){
        ClearScreen();
    }
    else if(StrCmp(command, QUIT)){
        shutdown();
    }
    CommandClear();
}