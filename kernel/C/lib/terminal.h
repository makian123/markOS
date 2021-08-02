#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"
#include "lists.h"

void CommandAdd(uint8_t ch);
void CommandPopBack();
void TermInit(void *stivPrint);

void printC(const uint8_t c);
int printS(const uint8_t *str);
int printF(const uint8_t *format, ...);
void printList(struct List *head);

void CommandClear();
uint8_t *GetCommand();
#endif