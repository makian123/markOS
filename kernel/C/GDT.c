#include "lib/gdt.h"

struct GDTEntry gdt[5];
struct GDTDescriptor gp;

extern void gdtFlush();

void GDTSetGate(int num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran){
    //Set bases
    gdt[num].baseLow = (base & 0xFFFF);
    gdt[num].baseMiddle = (base >> 16) & 0xFF;
    gdt[num].baseHigh = (base >> 24) & 0xFF;

    //Limit, granularity and access
    gdt[num].LimitLow = (limit & 0xFFFF);
    gdt[num].Granularity = (limit >> 16) & 0x0F;

    gdt[num].Granularity |= (gran & 0xF0);
    gdt[num].Access = access;
}

bool GDTInit(){
    //Set the gdt pointer values
    gp.size = (sizeof(struct GDTEntry) * 3) - 1;
    gp.offset = &gdt;

    GDTSetGate(0, 0, 0, 0, 0);   //Set null segment
    GDTSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);   //Set kernel code
    GDTSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);   //Set kernel data
    GDTSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);   //Set user code
    GDTSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);   //Set user data
    gdtFlush();
    return true;
}