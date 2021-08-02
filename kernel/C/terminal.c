#include "lib/terminal.h"
#include "lib/string.h"
#include <stdarg.h>

uint8_t *command = "aa\0";

void (*stivale_print)(const char *buf, size_t size) = null;
void printC(const uint8_t c){
    if (stivale_print != null)
        stivale_print(&c, 1);
    asm volatile ("outb %0, %1" :: "a" (c), "Nd" (0xe9) : "memory");
}
int printS(const uint8_t *str){
    uint32_t i = 0;
    int done = 1;
    while(i < StrLen(str)){
        printC(str[i]);
        i++;
    }
    return done;
}
int printF(const uint8_t *format, ...){
    va_list arg;
    int done;
    va_start(arg, format);
    while(*format != '\0'){
        if(*format != '%'){
            printC(*format);
            format++;
            continue;
        }
        format++;
        if(*format == '\0'){
            break;
        }
        switch(*format){
            case 's':
                printS(va_arg(arg, uint8_t*));
                break;
            case 'c':
                printC(va_arg(arg, int));
                break;
            case 'd':
                printS(itoa(va_arg(arg, int), 10));
                break;
            case 'i':
                printS(itoa(va_arg(arg, int), 10));
                break;
            case 'u':
                printS(itoa(va_arg(arg, uint32_t), 10));
                break;
            case 'p':
                printS("0x\0");
                printS(itoa(va_arg(arg, uint32_t), 16));
                break;
            case '%':
                printC('%');
                break;
        }
        format++;
    }
    va_end(arg);
    return done;
}
void printList(struct List *head){
    printC('[');
    while(ListNext(head) != null){
        printS(itoa(ListGetValue(head), 10));
        printS(", \0");
    }
    printC('\b');
    printC(']');
    printC('\n');
}

void CommandAdd(uint8_t ch){
    StringAdd(command, ch);
}

void CommandPopBack(){
    StrPopBack(command);
}

void TermInit(void *stivPrint){
    StrClear(command);
    stivale_print = stivPrint;
}

void CommandClear(){
    StrClear(command);
}

uint8_t *GetCommand(){
    return command;
}