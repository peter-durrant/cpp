#include "LinearRegression.h"

#include <iostream>

namespace hdd::gamma
{
    Regression LinearRegression(const valarray_fp& x, const valarray_fp& y)
    {
        double diffx;
        double diffy;
        double Mx = 0;
        double My = 0;
        double Qx = 0;
        double Qy = 0;
        double Qxy = 0;
        double xbar;
        double ybar;
        double sx;
        double sy;
        const uint32_t numNear = x.size();
        const double numNearFloatingPoint = numNear;

        for (uint32_t i = 1; i <= numNear; ++i)
        {
            const uint32_t j = i - 1;
            const double i_fp = i;
            diffx = x[j] - Mx;
            diffy = y[j] - My;
            Mx = ((i_fp - 1) * Mx + x[j]) / i_fp;
            My = ((i_fp - 1) * My + y[j]) / i_fp;
            Qx += (1.0 - (1.0 / i_fp)) * diffx * diffx;
            Qy += (1.0 - (1.0 / i_fp)) * diffy * diffy;
            Qxy += (1.0 - (1.0 / i_fp)) * diffx * diffy;
        }

        xbar = Mx;
        ybar = My;

        sx = sqrt(Qx / numNearFloatingPoint);
        sy = sqrt(Qy / numNearFloatingPoint);

        Regression regression;
        regression.gradient_ = Qxy / (numNearFloatingPoint * sx * sx);
        regression.intercept_ = ybar - (regression.gradient_ * xbar);

        if (numNear == 2)
        {
            regression.standardError_ = 0;
        }
        else
        {
            regression.standardError_ = sqrt((sy*sy) - (regression.gradient_ * regression.gradient_ * sx * sx));
        }

        return regression;
    }

    std::ostream& operator<<(std::ostream& os, const Regression& r)
    {
        os << "     Intercept = " << r.intercept_ << std::endl;
        os << "      Gradient = " << r.gradient_ << std::endl;
        os << "Standard error = " << r.standardError_ << std::endl;
        return os;
    }
}