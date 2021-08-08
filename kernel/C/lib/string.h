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
uint8_t * strpbrk(const uint8_t *str, const uint8_t *ct);
uint8_t * strsep(uint8_t **str, const uint8_t *ch);
void StrInsert(uint8_t *str, size_t pos, uint8_t ch);

uint8_t * itoa(long num, uint32_t base);
int atoi(uint8_t *str);
#endif