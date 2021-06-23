#ifndef STRING_H
#define STRING_H

#include "types.h"
#include "malloc.h"

void StrCpy(uint8_t *str, uint8_t *input);

void StringAdd(uint8_t *str, uint8_t ch);
size_t StrLen(const uint8_t *str);
void StrClear(uint8_t *str);
bool StrCmp(const uint8_t *str1, const uint8_t *str2);
void StrPopBack(uint8_t *str);
void StrReverse(uint8_t *str);

void itoa(uint8_t *str, uint64_t num);

#endif