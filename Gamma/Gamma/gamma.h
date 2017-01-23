#pragma once

#include "data.h"

struct Regression
{
    double intercept;
    double gradient;
    double standard_error;

    friend std::ostream& operator<<(std::ostream& os, const Regression& r);
};

Regression Linear_Regression(const valarray_fp& x, const valarray_fp& y);

class Gamma
{
public:
    Gamma(const Data& d, uint32_t p, uint32_t num_m = DefaultMoments());
    friend std::ostream& operator<<(std::ostream& os, const Gamma& g);
    static constexpr uint32_t DefaultMoments() { return 2; }

private:
    const Data& data;
    const uint32_t pmax;
    const uint32_t high_moments;

    std::vector<valarray_fp> delta;
    std::vector<std::vector<valarray_fp>> gamma;
    std::vector<std::vector<Regression>> r;
    std::vector<valarray_fp> moments;
};
