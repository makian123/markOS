#include "lib/pit.h"
#include "lib/idt.h"
#include "lib/system.h"

uint64_t tick;

static void PIT_Handler(/*/struct interrupt_registers *r*/)
{
    tick++;
}

void PITInit(uint64_t frequency){
    //register_interrupt_handler(IRQ0, PIT_Handler);

    uint64_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    uint8_t l = (uint8_t)(divisor & 0xFF), h = (uint8_t)(divisor >> 8) & 0xFF;

    outb(0x40, l);
    outb(0x40, h);
}