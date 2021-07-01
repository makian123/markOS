#include "lib/terminal.h"
#include "lib/types.h"
#include "lib/conversion.h"
#include "lib/keyboard.h"
#include "lib/kernel.h"
#include "lib/char.h"
#include "lib/string.h"
#include "lib/malloc.h"
#include "lib/snake.h"
#include "lib/system.h"
#include "lib/osMath.h"

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

    printChar('>', false, defaultColor);
    SnakeStart();
    bool isCaps = false;
    startInput(isCaps);
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