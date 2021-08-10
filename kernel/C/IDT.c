#include "lib/idt.h"
#include "lib/framebuffer.h"
#include "lib/malloc.h"
#include "lib/system.h"

typedef struct{
    uint64_t ds;
    uint64_t rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
    uint64_t intNo, errCode;
    uint64_t rip, cs, eflags, useresp, ss;
} registers;
idt_t idt[256];
idtr_t idtP;

#pragma region externs
extern void idtFlush();
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
#pragma endregion

uint8_t *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "Device not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault Exception",
    "General Protection Fault",
    "Page Fault",
    "[RESERVED]",
    "Floating Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point Exception",
    "Virtualization Exception",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "[RESERVED]",
    "Security Exception",
};

void idtSet(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags){
    //Set bases
    idt[num].BaseLow = (base & 0xFFFF);
    idt[num].BaseMiddle = (base >> 16) & 0xFFFF;
    idt[num].BaseHigh = (base >> 32) & 0xFFFFFFFF;

    //Set selector
    idt[num].Selector = sel;
    idt[num].Attr = flags;

    //Set reserved areas
    idt[num].IST = 0;
    idt[num].Zero = 0;
}

bool idtInit(){
    idtP.offset = (uint64_t)&idt;
    idtP.size = (sizeof(idt_t) * 256) - 1;

    memset((uint8_t*) &idt, 0, sizeof(idt_t) * 256);

    idtSet(0, (uint64_t)isr0, 0x08, 0x8E);
    idtSet(1, (uint64_t)isr1, 0x08, 0x8E);
    idtSet(2, (uint64_t)isr2, 0x08, 0x8E);
    idtSet(3, (uint64_t)isr3, 0x08, 0x8E);
    idtSet(4, (uint64_t)isr4, 0x08, 0x8E);
    idtSet(5, (uint64_t)isr5, 0x08, 0x8E);
    idtSet(6, (uint64_t)isr6, 0x08, 0x8E);
    idtSet(7, (uint64_t)isr7, 0x08, 0x8E);
    idtSet(8, (uint64_t)isr8, 0x08, 0x8E);
    idtSet(9, (uint64_t)isr9, 0x08, 0x8E);
    idtSet(10, (uint64_t)isr10, 0x08, 0x8E);
    idtSet(11, (uint64_t)isr11, 0x08, 0x8E);
    idtSet(12, (uint64_t)isr12, 0x08, 0x8E);
    idtSet(13, (uint64_t)isr13, 0x08, 0x8E);
    idtSet(14, (uint64_t)isr14, 0x08, 0x8E);
    idtSet(15, (uint64_t)isr15, 0x08, 0x8E);
    idtSet(16, (uint64_t)isr16, 0x08, 0x8E);
    idtSet(17, (uint64_t)isr17, 0x08, 0x8E);
    idtSet(18, (uint64_t)isr18, 0x08, 0x8E);
    idtSet(19, (uint64_t)isr19, 0x08, 0x8E);
    idtSet(20, (uint64_t)isr20, 0x08, 0x8E);
    idtSet(21, (uint64_t)isr21, 0x08, 0x8E);
    idtSet(22, (uint64_t)isr22, 0x08, 0x8E);
    idtSet(23, (uint64_t)isr23, 0x08, 0x8E);
    idtSet(24, (uint64_t)isr24, 0x08, 0x8E);
    idtSet(25, (uint64_t)isr25, 0x08, 0x8E);
    idtSet(26, (uint64_t)isr26, 0x08, 0x8E);
    idtSet(27, (uint64_t)isr27, 0x08, 0x8E);
    idtSet(28, (uint64_t)isr28, 0x08, 0x8E);
    idtSet(29, (uint64_t)isr29, 0x08, 0x8E);
    idtSet(30, (uint64_t)isr30, 0x08, 0x8E);
    idtSet(31, (uint64_t)isr31, 0x08, 0x8E);

    idtFlush();
    return true;
}

void isrHandler(uint64_t exNo, uint64_t rsp){
    (void)rsp;
    glog(FAILURE, "FAILURE: %s", exception_messages[exNo]);
    hang();
}