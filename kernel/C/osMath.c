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

    int start = 0;
    int end = input / 2;
    int mid = ((start + end) / 2) <= 1 ? 1 : (start + end) / 2;

    while(start < end && mid){
        int minVal = input / mid;
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

int floor(float input){
    return (int)input;
}

int ceil(float input){
    if(input < 0){
        return (int)input + 1;
    }
    return (int)input;
}

int round(float input){
    if(input - (int)input >= 0.5) return ceil(input);
    else return floor(input);
}
uint32_t rand(){
    next = next * 1103515245 + 12345;
    return (unsigned)(next/65536) % 32768;
}