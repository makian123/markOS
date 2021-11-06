#pragma once
#include <stivale2.h>

void* stivale2_get_tag(stivale2_struct *stivale, uint64_t id);

struct CRs
{
    uint64_t cr0;
    uint64_t cr2;
    uint64_t cr3;
};
