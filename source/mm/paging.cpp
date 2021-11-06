#include "paging.hpp"

void PageDirEntry::SetFlag(PT_Flags flag, bool enabled)
{
    uint64_t bitSel = (uint64_t)1 << flag;
    val &= ~bitSel;
    if (enabled) val |= bitSel;
}

bool PageDirEntry::GetFlag(PT_Flags flag)
{
    uint64_t bitSel = (uint64_t)1 << flag;
    return (val & (bitSel > 0)) ? true : false;
}

uint64_t PageDirEntry::GetAddress()
{
    return (val & 0x000FFFFFFFFFF000) >> 12;
}

void PageDirEntry::SetAddress(uint64_t address)
{
    address &= 0x000000FFFFFFFFFF;
    val &= 0xFFF0000000000FFF;
    val |= (address << 12);
}