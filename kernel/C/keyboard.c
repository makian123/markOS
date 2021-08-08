#include "lib/keyboard.h"
#include "lib/framebuffer.h"
#include "lib/system.h"

uint8_t keyboard_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\0',
    '\t',
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0,
    '*',
    0,
    ' ',
    0,
    0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0,
    0,
    0,
    '-',
    0,
    0,
    0,
    '+',
    0,
    0,
    0,
    0,
    0,
    0, 0, 0,
    0,
    0,
    0
};

void KBInit(){
    PrintS("Keyboard initialized!\n");
}

void KBMain(){
    uint8_t state = inb(0x64);
    while(state & 1 && (state & 0x20) == 0){
        uint8_t keyCode = inb(0x60);
        uint8_t scanCode = keyCode & 0x07;
        uint8_t keyState = !(keyCode & 0x80);

        if(keyState){
            PrintF("%c", keyboard_map[(uint8_t)scanCode]);
        }
        state = inb(0x64);
    }
    outb(0x20, 0x20);
}