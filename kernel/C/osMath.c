#include "lib/osMath.h"
#include "lib/types.h"

uint32_t powM(uint32_t a, uint32_t b){
    uint32_t output = 1;
    while(b > 0){
        output *= a;
        b--;
    }
    return output;
}