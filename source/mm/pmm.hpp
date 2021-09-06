#pragma once

#include <include/types.hpp>
#include <stivale2.h>

void pmm_init(stivale2_mmap_entry *memap, size_t memap_entries);
void pmm_reclaim_memory(stivale2_mmap_entry *memmap, size_t memap_entries);
void *pmm_alloc(size_t count);
void *pmm_allocz(size_t count);
void pmm_free(void *ptr, size_t count);