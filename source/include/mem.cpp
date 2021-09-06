#include "mem.hpp"

void *memset(void *dest, int val, size_t size){
    uint8_t *s = (uint8_t*)dest;
    while(size-- > 0){
        *s = val;
        s++;
    }

    return dest;
}

void *mempcpy(void *dest, const void *src, size_t nbytes) {
	return (char *)memcpy(dest, src, nbytes) + nbytes;
}