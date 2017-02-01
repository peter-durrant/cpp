#pragma once

#include "types.h"

namespace hdd::gamma
{
    struct Regression
    {
        double intercept_;
        double gradient_;
        double standardError_;

        friend std::ostream& operator<<(std::ostream& os, const Regression& r);
    };

    Regression LinearRegression(const valarray_fp& x, const valarray_fp& y);
}
