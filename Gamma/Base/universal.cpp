#include "universal.h"

const unsigned int EMPTY = 0;

const fp fp_ZERO = 0;
const fp fp_MIN = std::numeric_limits<fp>::min();
const fp fp_MAX = std::numeric_limits<fp>::max();

fp Raise(fp value, uint power) {
   fp total = 1;
   for (uint i = 0; i < power; i++) total = value * total;
   return total;
}

