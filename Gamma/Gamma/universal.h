#pragma once

#include <string>
#include <sstream>
#include <valarray>
#include <vector>
#include <map>
#include <iostream>

typedef unsigned int uint;
typedef double fp;

extern const unsigned int EMPTY;

typedef std::vector<fp> vect_fp;
typedef std::valarray<fp> valarray_fp;
typedef std::vector<uint> vect_uint;
typedef std::valarray<uint> valarray_uint;

extern const fp fp_ZERO;
extern const fp fp_MIN;
extern const fp fp_MAX;

fp Raise(fp value, uint power);
