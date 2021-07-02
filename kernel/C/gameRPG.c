#include "lib/gameRPG.h"
#include "lib/osMath.h"
#include "lib/malloc.h"
#include "lib/string.h"
#include "lib/system.h"
#include "lib/char.h"
#include "lib/terminal.h"

struct player *User;
uint8_t *command;
bool isPlaying;

void gameQuit(){
    isPlaying = false;

    newInputLine();
}
void AddToCommand(uint8_t ch){
    StringAdd(command, ch);
}
void GamePopBack(){
    StrPopBack(command);
}

void startIn(){
    uint8_t ch = 0;
    bool running = true;
    uint8_t keycode = 0;
    do{
        keycode = getInput();
        switch (keycode)
        {
            case KEY_ENTER:
                if(StrLen(command) > 0){
                    newInputLine();
                    running = false;
                }
                break;

            case KEY_BACKSPACE:
                GamePopBack();
                break;

            default:
                ch = get_ascii_char(keycode, false);
                printChar(ch, false, termColor(COLOR_WHITE, COLOR_BLACK));
                AddToCommand(ch);
                break;
        }
        ch = 0;
        waitForIO(0x02FFFFFF);
    }while(running);
}

void GameInit(){
    //Player allocation
    User = (struct player*) Malloc(sizeof(struct player*));
    User->HP = 100;
    User->damage = 10;
    User->defense = 0;
    User->name = "asd\0";

    isPlaying = true;
    command = "\0";

    printString("What will be your name?\0");
    newInputLine();

    startIn();
    printString("Your name is: \0");
    printString(command);
    newInputLine();

    while(isPlaying){
        printString("Asd\0");
        gameQuit();
    }
}