#pragma once

#include "types.hpp"

uint8_t *itoa(long num, uint8_t *buffer, int base);
uint8_t *itoa(uint64_t num, uint8_t *buffer, int base);
uint8_t *reverse(uint8_t *buffer, int i, int j);
const uint8_t* to_hstring(uint64_t value);