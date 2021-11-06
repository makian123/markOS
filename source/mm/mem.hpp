#pragma once

#include <include/types.hpp>
#include <boot/bootVars.hpp>

void *memset(void *dest, int val, size_t len);
void *memcpy(void *dest, void *src, size_t len);
int memcmp(void *first, void *second, size_t len);

uint64_t getMemSize(stivale2_struct_tag_memmap *memmap);