#include "lib/terminal.h"
#include "lib/types.h"
#include "lib/conversion.h"
#include "lib/keyboard.h"
#include "lib/kernel.h"
#include "lib/char.h"
#include "lib/string.h"


void kmain(void){
    termInit();
    termClear();
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
    uint16_t pos = 10 * TERM_WIDTH + 3;
    /*outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));*/
    //Input
    struct string *temp;
    StringCreate(temp, "asd");
    StringAdd(temp, 'L');
    printString(temp);
    printChar('>', false);
    bool isCaps = true;
    startInput(isCaps);
}

uint8_t inb(uint16_t port){
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
    return ret;
}

void outb(uint16_t port, uint8_t data){
    asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
}
uint8_t getInput(){
    char ch = 0;
    while((ch = inb(KEYBOARD_PORT)) != 0){
        if(ch > 0)
            return ch;
        }
    return ch;
}
void waitForIO(uint32_t timerCount){
    while(1){
    asm volatile("nop");
    timerCount--;
    if(timerCount <= 0)
        break;
    }
}
void sleep(uint32_t timerCount){
    waitForIO(timerCount);
}

void startInput(bool* isCaps){
    uint8_t ch = 0;
    uint8_t keycode = 0;
    do{
        keycode = getInput();
        switch (keycode)
        {
        case KEY_ENTER:
            //commandExecute();
            newInputLine();
            break;
        case KEY_BACKSPACE:
            backspace();
            break;
        case KEY_TAB:
            *isCaps = !*isCaps;
            break;

        default:
            ch = get_ascii_char(keycode, *isCaps);
            printChar(ch, true);
            break;
        }
        sleep(0x02FFFFFF);
    }while(ch > 0);
}