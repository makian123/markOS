#include "lib/IDT.h"
#include "lib/terminal.h"

__attribute__((noreturn))
void exception_handler() {
    __asm__ volatile ("cli; hlt");
}

bool vectors[256];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_desc_t* descriptor = &idt[vector];
 
    descriptor->base_low       = (uint64_t)isr & 0xFFFF;
    descriptor->cs             = 0x28;
    descriptor->ist            = 0;
    descriptor->attributes     = flags;
    descriptor->base_mid       = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->base_high      = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->rsv0           = 0;
}

bool IDTInit(){
    printS("Initializing IDT...\n");
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_desc_t) * 256 - 1;
 
    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }
    __asm__ volatile ("lidt %0" : : "memory"(idtr));
    __asm__ volatile ("sti");
    printS("IDT inizialized\n");
}