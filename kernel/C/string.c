#include "lib/string.h"
#include "lib/osMath.h"

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
    size_t i = 0, j = StrLen(str) - 1;
    uint8_t temp;
    while(i < j){
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

void StrInsert(uint8_t *str, size_t pos, uint8_t ch){
    if(pos > StrLen(str)) return;
    StringAdd(str, '0');
    for(size_t i = StrLen(str) - 1; i > pos; i--){
        str[i] = str[i - 1];
    }
    str[pos] = ch;
}

uint8_t * itoa(long num, uint32_t base){
    if(num == 0) return "0\0";

    uint8_t *output = '\0';
    bool isNegative = num < 0;

    if(isNegative) num = num * -1;
    StrClear(output);
    while(num){
        uint32_t r = num % base;
        if(r >= 10) StringAdd(output, (65 + (r - 10)));
        else StringAdd(output, (48 + r));

        num = num / base;
    }

    if(isNegative) StringAdd(output, '-');

    StrReverse(output);
    return output;
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