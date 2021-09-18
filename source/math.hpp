#pragma once

#include <include/types.hpp>

#define abs(x) x > 0 ? x : -x
#define sign(x) (x > 0 ? 1 : (x < 0 ? -1 : 0))

int pow(int base, int exponent);