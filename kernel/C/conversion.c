#include "lib/conversion.h"
#include "lib/types.h"

uint8_t intToChar(uint16_t input){
    if(input < 0 || input > 9) return '0';
    return (uint8_t)(input + 48);
}

uint16_t charToInt(uint8_t input){
    if(input < 48 || input > 57) return (uint16_t)0;
    return (uint16_t)(input - 48);
}