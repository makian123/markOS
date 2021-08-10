#ifndef CPU_H
#define CPU_H

#include "types.h"

int cpu_current_logical_processor_id();
void cpu_vendor_string(char* buffer);
int max_cpuid_leaf();


#define SEGMENT_TABLE_LDT (1<<2)
#define SEGMENT_RPL_KERNEL (0)
#define SEGMENT_RPL_USER (3)
#define SEGMENT_INDEX(seg) ((seg & 0x7) << 3)



extern void asm_load_cs(uint64_t selector);
extern void asm_load_ds(uint16_t selector);
extern void asm_load_es(uint16_t selector);
extern void asm_load_fs(uint16_t selector);
extern void asm_load_gs(uint16_t selector);
extern void asm_load_ss(uint16_t selector);


#endif