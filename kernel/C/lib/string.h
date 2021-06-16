#ifndef STRING_H
#define STRING_H

#include "types.h"
#include "malloc.h"

typedef struct string{
    uint8_t data;
    struct string *next;
}string;

void StringCreate(struct string *str, uint8_t *arr);

size_t StringLen(struct string *str);
uint8_t StringGetAt(struct string *str, size_t pos);
void StringAdd(struct string *str, uint8_t ch);
void StringAddFirst(struct string *str, uint8_t ch);

#endif