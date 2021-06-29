#include "lib/terminal.h"
#include "lib/types.h"
#include "lib/conversion.h"
#include "lib/keyboard.h"
#include "lib/kernel.h"
#include "lib/char.h"
#include "lib/string.h"
#include "lib/malloc.h"

uint8_t defaultColor;

//Solves the stack check fail error when parsing addresses
extern void __stack_chk_fail(void){
}

void kmain(void){
    defaultColor = termColor(COLOR_GREEN, COLOR_BLACK);
    termInit();
    termClear();
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
    uint16_t pos = 10 * TERM_WIDTH + 3;
    outb(REG_CTR, 15);
    outb(REG_CTR, (uint8_t)(14 >> 8));
    /*outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));*/

    //Input done
    printChar('>', false, defaultColor);
    printString(itoa(1234567890, 10));
    bool isCaps = false;
    startInput(isCaps);
}


//ASM functions in C
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

void startInput(bool *isCaps){
    uint8_t ch = 0;
    uint8_t keycode = 0;
    do{
        keycode = getInput();
        switch (keycode)
        {
        case KEY_ENTER:
            ExecuteCommand();
            newInputLine();
            break;
        case KEY_BACKSPACE:
            CommandPopBack();
            backspace();
            break;
        case KEY_TAB:
            *isCaps = !*isCaps;
            break;

        default:
            ch = get_ascii_char(keycode, *isCaps);
            printChar(ch, true, defaultColor);
            CommandAdd(ch);
            break;
        }
        sleep(0x02FFFFFF);
    }while(ch > 0);
}