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

    while(n){
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