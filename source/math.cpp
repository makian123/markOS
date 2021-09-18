#include "math.hpp"

int pow(int base, int exponent){
    int res = 1;

    while(exponent-- > 0){
        res *= base;
    }

    return res;
}