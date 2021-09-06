#pragma once

#include "types.hpp"

void *alloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t new_size);