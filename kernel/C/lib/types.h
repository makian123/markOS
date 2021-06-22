#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint64_t size_t;

typedef int int32;
typedef long long int64;

//typedef uint32_t size_t;
typedef enum{
    false = (1 == 0),
    true = (! false)
} bool;


#define null 0

#endif