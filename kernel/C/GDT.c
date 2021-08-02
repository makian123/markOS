#include "lib/GDT.h"
#include "lib/terminal.h"

__attribute__((aligned(0x1000)))
struct GDT DefaultGDT = {
    {0, 0, 0, 0x00, 0x00, 0},
    {0xffff, 0, 0, 0x9a, 0x80, 0},
    {0xffff, 0, 0, 0x92, 0x80, 0},
    {0xffff, 0, 0, 0x9a, 0xcf, 0},
    {0xffff, 0, 0, 0x92, 0xcf, 0},
    {0, 0, 0, 0x9a, 0xa2, 0},
    {0, 0, 0, 0x92, 0xa0, 0}
};

bool GDTInit()
{
    printS("Initializing GDT...\n");
    struct GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(struct GDT) - 1;
    gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
    LoadGDT(&gdtDescriptor);
    printS("GDT Initialized\n");
    return true;
}