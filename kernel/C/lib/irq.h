#ifndef IRQ_H
#define IRQ_H

#include "types.h"

void installHandler(int irq, void (*handler)());
void uninstallHandler(int irq);
bool initIrq();

#endif