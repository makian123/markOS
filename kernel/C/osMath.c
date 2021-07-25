#include "lib/osMath.h"
#include "lib/types.h"

static unsigned long next = 1;

uint32_t pow(uint32_t base, uint32_t exponent){
    uint32_t output = 1;
    while(exponent > 0){
        output *= base;
        exponent--;
    }
    return output;
}

double sqrt(double input){
    if(input <= 0) return 0;
    if(input == -0) return -0;

    sint32_t start = 0;
    sint32_t end = input / 2;
    sint32_t mid = ((start + end) / 2) <= 1 ? 1 : (start + end) / 2;

    while(start < end && mid){
        sint32_t minVal = input / mid;
        if(minVal == mid) return mid;
        if(minVal < mid){
            end = mid - 1;
        }
        else{
            start = mid + 1;
        }
        mid = (start + end) / 2;
    }

    if(mid * mid > input) mid--;
    return mid;
}

uint32_t rand(){
    next = next * 1103515245 + 12345;
    return (unsigned)(next/65536) % 32768;
}