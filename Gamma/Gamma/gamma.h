#pragma once

#include "data.h"

extern const uint moments_MIN;
extern const uint moments_MAX;
extern const uint default_moments;

struct Regression
{
    fp intercept;
    fp gradient;
    fp standard_error;

    friend std::ostream& operator<<(std::ostream& os, const Regression& r);
};

Regression Linear_Regression(const valarray_fp& x, const valarray_fp& y);

class Gamma
{
private:
    const Data& data;
    const uint pmax;
    const uint high_moments;

    std::vector<valarray_fp> delta;
    std::vector<std::vector<valarray_fp> > gamma;
    std::vector<std::vector<Regression> > r;
    std::vector<valarray_fp> moments;

public:
    Gamma(const Data& d, uint p, uint num_m = default_moments);
    friend std::ostream& operator<<(std::ostream& os, const Gamma& g);
};
