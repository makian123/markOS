#include "smp.hpp"


void smpInit(stivale2_struct_tag_smp *smp){
    uint8_t bspLapicID = smp->bsp_lapic_id;

    for(size_t i = 0; i < smp->cpu_count; ++i){
        if(smp->smp_info[i].lapic_id == bspLapicID){
            continue;
        }

    
    }
}