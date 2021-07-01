#include "lib/osMath.h"
#include "lib/types.h"

static unsigned long next = 1;

uint32_t powM(uint32_t a, uint32_t b){
    uint32_t output = 1;
    while(b > 0){
        output *= a;
        b--;
    }
    return output;
}

uint32_t rand(){
    next = next * 1103515245 + 12345;
    return (unsigned)(next/65536) % 32768;
}