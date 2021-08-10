#include "lib/cpu.h"

void cpu_vendor_string(uint8_t* buf) {
    uint32_t eax, ebx, ecx, edx;
    cpuid(0x0, &eax, &ebx, &ecx, &edx);
    // Start with ebx
    for (uint8_t i = 0; i < 4; i++) {
        *buf = (uint8_t)(ebx>>(i*8));
        ++buf;
    }
    // Then edx
    for (uint8_t i = 0; i < 4; i++) {
        *buf = (uint8_t)(edx>>(i*8));
        ++buf;
    }
    // Then ecx
    for (uint8_t i = 0; i < 4; i++) {
        *buf = (uint8_t)(ecx>>(i*8));
        ++buf;
    }
}

int max_cpuid_leaf() {
    uint32_t eax, unused;
    cpuid(0, &eax, &unused, &unused, &unused);
    return eax;
}

int cpu_current_logical_processor_id() {
    ///////
    //TODO: Use x2APIC ID
    ///////
    uint32_t ebx, unused;
    cpuid(0x1, &unused, &ebx, &unused, &unused);
    // CPUID 0x1: EBX:
    // EBX Bits 31-24: Initial APIC ID
    return (int)(ebx>>24);
}
