#ifndef LISTS_H
#define LISTS_H

#include "types.h"

struct List;

void ListCreate(struct List *head, long value);
void ListAdd(struct List *head, long toAdd);
long ListGetAt(struct List *head, size_t pos);
void ListInsert(struct List *head, size_t pos, long val);

long ListGetValue(struct List *node);
struct List *ListNext(struct List *node);
#endif