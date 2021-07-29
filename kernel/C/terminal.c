#include "lib/terminal.h"
#include "lib/string.h"

uint8_t *command = "aassa\0";

void CommandAdd(uint8_t ch){
    StringAdd(command, ch);
}

void CommandPopBack(){
    StrPopBack(command);
}

void TermInit(){
    StrClear(command);
}

void CommandExecute(){
    if(StrCmp(command, "clear\0")){

    }

    StrClear(command);
}

uint8_t *GetCommand(){
    return command;
}