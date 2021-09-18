#pragma once

#include <include/types.hpp>
#include <boot/bootVars.hpp>

#define GENERIC_CAST(type, var) ((type) var)
#define MM_BASE  0xffffffff80000000 
#define VMM_BASE 0xffff800000000000
#define PAGE_SIZE   4096

void *memset(void *dest, int val, size_t len);
void *memcpy(void *dest, void *src, size_t len);
int memcmp(void *first, void *second, size_t len);

uint64_t getUsableMemSize(boot_info_t *bootInfo);
uint64_t getMemSize(boot_info_t *bootInfo);