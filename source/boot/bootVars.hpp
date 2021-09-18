#pragma once

#include <stivale2.h>
#include <include/types.hpp>

typedef struct boot_cpu {
    uint64_t  processor_count;
    uint32_t  acpi_processor_uid;
    uint32_t  lapic_id;
    uint32_t  bootstrap_processor_lapic_id;
} boot_cpu_t;

typedef struct boot_mmap {
    uint64_t entries;
    uint64_t base;
    uint64_t length;
    uint32_t type;
    struct stivale2_mmap_entry *memmap;
} boot_mmap_t;

typedef struct boot_info {
    boot_cpu_t  cpu;
    boot_mmap_t mmap;
    void *rdsp;
} boot_info_t;