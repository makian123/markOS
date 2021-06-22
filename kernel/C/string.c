#include "lib/string.h"
#include "lib/terminal.h"

size_t StrLen(uint8_t *str){
    size_t len = 0;
    while(str[len] != '\0'){
        len++;
    }
    return len;
}

void StringAdd(uint8_t *str, uint8_t ch){
    str[StrLen(str)] = ch;
    str[StrLen(str) + 1] = (uint8_t*) Malloc(sizeof(uint8_t));
    str[StrLen(str) + 1] = '\0';
}

void StrClear(uint8_t *str){
    size_t len = StrLen(str);
    str[0] = '\0';
    for(size_t i = len - 1; i > 0; --i){
        str[i] = null;
    }
    len = null;
}

bool StrCmp(uint8_t *str1, uint8_t *str2){
    if(StrLen(str1) != StrLen(str2)) return false;
    for(size_t i = 0; i < StrLen(str1); ++i){
        if(str1[i] != str2[i]) return false;
    }

    return true;
}

void StrPopBack(uint8_t *str){
    size_t len = StrLen(str);
    str[len - 1] = '\0';
    str[len] = null;
}

void StrReverse(uint8_t *str){
    uint8_t temp = '\0';
    size_t len = StrLen(str);
    for(size_t i = 0; i <= (size_t)(len / 2); ++i){
        temp = str[i];
        str[i] = str[len - (1 + i)];
        str[len - (1 + i)] = temp;
    }
    temp = null;
}

void itoa(uint8_t *str, uint64_t num){
    StrClear(str);
    uint8_t *temp = "\0";
    while(num > 0){
        StringAdd(temp, (uint8_t)((num % 10) + '0'));
        num = num / 10;
    }
    str = temp;
    StrClear(temp);
}