#pragma once
#include <include/types.hpp>

uint8_t inb(uint16_t port);

void outb(uint16_t port, uint8_t val);

void io_wait(void);

bool are_interrupts_enabled(bool should_print);