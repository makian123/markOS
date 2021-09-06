#pragma once

#include <include/types.hpp>

#define abs(x) x > 0 ? x : -x

#define DIV_ROUNDUP(A, B) (A + B - 1) / B

#define ALIGN_UP(A, B) DIV_ROUNDUP(A, B) * B
#define ALIGN_DOWN(A, B) (A / B) * B
