#include "lib/string.h"
#include "lib/terminal.h"

size_t StrLen(const uint8_t *str){
    size_t len = 0;
    while(str[len] != '\0'){
        ++len;
    }
    return len;
}

void StringAdd(uint8_t *str, uint8_t ch){
    str[StrLen(str)] = ch;
    str[StrLen(str) + 1] = (uint8_t*) Malloc(sizeof(uint8_t));
    str[StrLen(str) + 1] = '\0';
}

void StrCpy(uint8_t *str, uint8_t *input){
    StrClear(str);
    for(size_t i = 0; i < StrLen(input); ++i){
        StringAdd(str, input[i]);
    }
}

void StrClear(uint8_t *str){
    size_t len = StrLen(str);
    str[0] = '\0';
    for(size_t i = len - 1; i > 0; --i){
        str[i] = null;
    }
    len = null;
}

bool StrCmp(const uint8_t *str1, const uint8_t *str2){
    while(*str1 == *str2 && *str1 != '\0'){
        str1++;
        str2++;
    }

    return (*str1 > *str2) - (*str1 < *str2) == 0 ? true : false;
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

uint8_t * strpbrk(const uint8_t *str, const uint8_t *ct){
    const uint8_t *sc1, *sc2;
    for(sc1 = str; *sc1 != '\0'; ++sc1){
        for (sc2 = ct; *sc2 != '\0'; ++sc2) {
			if (*sc1 == *sc2)
				return (char *)sc1;
		}
    }
    return null;
}

uint8_t * strsep(uint8_t **str, const uint8_t *ch){
    uint8_t *sBegin = *str;
    uint8_t *end;

    if(sBegin == null){
        return null;
    }
    end = strpbrk(sBegin, ch);
    if(end){
        *end++ = '\0';
    }
    *str = end;
    return sBegin;
}