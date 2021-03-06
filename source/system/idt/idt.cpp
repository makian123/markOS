#include <include/io.hpp>
#include <system/idt/idt.hpp>

int_handler_t interrupt_handlers[256];
Framebuffer *mainFB;

void idt_set_descriptor(uint8_t vector, void* isr)
{
    idtDesc_t* descriptor = (idtDesc_t *)&idt[vector];

    descriptor->offset_1 = (uint64_t)isr & 0xFFFF;
    descriptor->selector = 0x28;
    descriptor->ist = 0;
    descriptor->type_attr = 0x8E;
    descriptor->offset_2 = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->offset_3 = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->zero = 0;
}

void registerInterrupt(uint8_t n, int_handler_t handler);
static void not_implemented(struct interrupt_registers *);
void installISR();

void initIDT(Framebuffer *fb)
{
    mainFB = fb;
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)((sizeof(idtDesc_t) * 256) - 1);

    for (int i = 32; i < 48; i++)
    {
        registerInterrupt(i, not_implemented);
    }

    installISR();

    __asm__ volatile ("lidt %0" : : "memory"(idtr));
    __asm__ volatile ("sti");
}

void registerInterrupt(uint8_t n, int_handler_t handler)
{
    interrupt_handlers[n] = handler;
}

static const char *exception_messages[32] = {
    "Divide by zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Overflow",
    "Bounc range exceeded",
    "Invalid opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack segment fault",
    "General protection fault",
    "Page fault",
    "Reserved",
    "x87 Floating point exception",
    "Alignment check",
    "Machine check",
    "SIMD Floating point exception",
    "Virtualization exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Security exception",
    "Reserved",
};

void isrHandler(struct interrupt_registers *regs)
{
    mainFB->Log(ERROR, (char*)exception_messages[regs->int_no]);
    mainFB->PrintF(" at line: %x", regs->rip);
    __asm__ volatile ("cli; hlt");
}

void irqHandler(struct interrupt_registers *regs)
{
    if (regs->int_no != 0)
    {
        int_handler_t handler = interrupt_handlers[regs->int_no];
        handler(regs);
    }

    if(regs->int_no >= 8)
    {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}

static void not_implemented(struct interrupt_registers *)
{
}

void installISR()
{
    idt_set_descriptor(0, (void*)isr0);
    idt_set_descriptor(1, (void*)isr1);
    idt_set_descriptor(2, (void*)isr2);
    idt_set_descriptor(3, (void*)isr3);
    idt_set_descriptor(4, (void*)isr4);
    idt_set_descriptor(5, (void*)isr5);
    idt_set_descriptor(6, (void*)isr6);
    idt_set_descriptor(7, (void*)isr7);
    idt_set_descriptor(8, (void*)isr8);
    idt_set_descriptor(9, (void*)isr9);
    idt_set_descriptor(10, (void*)isr10);
    idt_set_descriptor(11, (void*)isr11);
    idt_set_descriptor(12, (void*)isr12);
    idt_set_descriptor(13, (void*)isr13);
    idt_set_descriptor(14, (void*)isr14);
    idt_set_descriptor(15, (void*)isr15);
    idt_set_descriptor(16, (void*)isr16);
    idt_set_descriptor(17, (void*)isr17);
    idt_set_descriptor(18, (void*)isr18);
    idt_set_descriptor(19, (void*)isr19);
    idt_set_descriptor(20, (void*)isr20);
    idt_set_descriptor(21, (void*)isr21);
    idt_set_descriptor(22, (void*)isr22);
    idt_set_descriptor(23, (void*)isr23);
    idt_set_descriptor(24, (void*)isr24);
    idt_set_descriptor(25, (void*)isr25);
    idt_set_descriptor(26, (void*)isr26);
    idt_set_descriptor(27, (void*)isr27);
    idt_set_descriptor(28, (void*)isr28);
    idt_set_descriptor(29, (void*)isr29);
    idt_set_descriptor(30, (void*)isr30);
    idt_set_descriptor(31, (void*)isr31);

    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    idt_set_descriptor(32, (void*)irq0);
    idt_set_descriptor(33, (void*)irq1);
    idt_set_descriptor(34, (void*)irq2);
    idt_set_descriptor(35, (void*)irq3);
    idt_set_descriptor(36, (void*)irq4);
    idt_set_descriptor(37, (void*)irq5);
    idt_set_descriptor(38, (void*)irq6);
    idt_set_descriptor(39, (void*)irq7);
    idt_set_descriptor(40, (void*)irq8);
    idt_set_descriptor(41, (void*)irq9);
    idt_set_descriptor(42, (void*)irq10);
    idt_set_descriptor(43, (void*)irq11);
    idt_set_descriptor(44, (void*)irq12);
    idt_set_descriptor(45, (void*)irq13);
    idt_set_descriptor(46, (void*)irq14);
    idt_set_descriptor(47, (void*)irq15);
}
