#include "string.hpp"
#include <math.hpp>

uint8_t *reverse(uint8_t *buffer, int i, int j){
    if(i > j){
        i, j = j, i;
    }
    uint8_t temp;
    while(i < j){
        temp = buffer[i];
        buffer[i++] = buffer[j];
        buffer[j--] = temp;
    }

    return buffer;
}

uint8_t *itoa(long num, uint8_t *buffer, int base = 10){
    if(base < 2 || base > 32){
        return buffer;
    }

    int n = abs(num);

    int i = 0;

    while(n > 0){
        int reminder = n % base;

        if(reminder >= 10){
            buffer[i++] = 65 + (reminder - 10);
        }
        else{
            buffer[i++] = 48 + reminder;
        }

        n /= base;
    }

    if(i == 0){
        buffer[i++] = '0';
    }

    if(num < 0 && base == 10){
        buffer[i++] = '-';
    }
    buffer[i] = '\0';

    return reverse(buffer, 0, i - 1);
}

uint8_t *itoa(uint64_t num, uint8_t *buffer, int base){
    if(base < 2 || base > 32){
        return buffer;
    }

    int i = 0;

    while(num > 0){
        int reminder = num % base;

        if(reminder >= 10){
            buffer[i++] = 65 + (reminder - 10);
        }
        else{
            buffer[i++] = 48 + reminder;
        }

        num /= base;
    }

    if(i == 0){
        buffer[i++] = '0';
    }
    buffer[i] = '\0';

    return reverse(buffer, 0, i - 1);
}

uint8_t hexTo_StringOutput[128];
const uint8_t* to_hstring(uint64_t value){
    uint64_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 - 1;
    for (uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }
    hexTo_StringOutput[size + 1] = 0;
    return hexTo_StringOutput;
}