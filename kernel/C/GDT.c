#include "lib/gdt.h"

__attribute__((alligned(0x1000)))

struct GDT DefaultGDT = {
    {0, 0, 0, 0x00, 0x00, 0},
    {0, 0, 0, 0x9A, 0xA0, 0},
    {0, 0, 0, 0x92, 0xA0, 0},
    {0, 0, 0, 0x00, 0x00, 0},
    {0, 0, 0, 0x9A, 0xA0, 0},
    {0, 0, 0, 0x92, 0xA0, 0}
};