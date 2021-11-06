#pragma once

#include <include/types.hpp>

enum PT_Flags{
    Present = 0,
    RW = 1,
    SuperUser = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    ignore1 = 6,
    LargerPages = 7,
    Custom0 = 9,
    Custom1 = 10,
    Custom2 = 11,
    NX = 63
};

struct PageDirEntry{
    uint64_t val;
    void SetFlag(PT_Flags flag, bool enabled);
    bool GetFlag(PT_Flags flag);
    void SetAddress(uint64_t address);
    uint64_t GetAddress();
};

struct PageTable{
    PageDirEntry entries[512];
}__attribute__((aligned(0x1000)));
