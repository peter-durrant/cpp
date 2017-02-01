#include "stdafx.h"
#include "gtest/gtest.h"
#include "linearregression.h"

namespace GammaLibTest
{
    TEST(LinearRegression, StraightLineZeroIntercept)
    {
        // arrange
        valarray_fp x{ 1, 2, 3, 4, 5, 6 };
        valarray_fp y{ 10, 20, 30, 40, 50, 60 };

        // act
        auto regression = hdd::gamma::LinearRegression(x, y);

        // assert
        EXPECT_EQ(10, regression.gradient_);
        EXPECT_EQ(0, regression.intercept_);
        EXPECT_NEAR(0, regression.standardError_, 1e-06);
    }

    TEST(LinearRegression, StraightLineInterceptPositive)
    {
        // arrange
        valarray_fp x{ 1, 2, 3, 4, 5, 6 };
        valarray_fp y{ 11, 12, 13, 14, 15, 16 };

        // act
        auto regression = hdd::gamma::LinearRegression(x, y);

        // assert
        EXPECT_EQ(1, regression.gradient_);
        EXPECT_EQ(10, regression.intercept_);
        EXPECT_NEAR(0, regression.standardError_, 1e-06);
    }

    TEST(LinearRegression, StraightLineNegativeGradientInterceptNegative)
    {
        // arrange
        valarray_fp x{ 1, 2, 3, 4, 5, 6 };
        valarray_fp y{ -10, -11, -12, -13, -14, -15 };

        // act
        auto regression = hdd::gamma::LinearRegression(x, y);

        // assert
        EXPECT_EQ(-1, regression.gradient_);
        EXPECT_EQ(-9, regression.intercept_);
        EXPECT_NEAR(0, regression.standardError_, 1e-06);
    }

    TEST(LinearRegression, StraightLineNoGradient)
    {
        // arrange
        valarray_fp x{ 1, 2, 3, 4, 5, 6 };
        valarray_fp y{ 5, 5, 5, 5, 5, 5 };

        // act
        auto regression = hdd::gamma::LinearRegression(x, y);

        // assert
        EXPECT_EQ(0, regression.gradient_);
        EXPECT_EQ(5, regression.intercept_);
        EXPECT_NEAR(0, regression.standardError_, 1e-06);
    }

    TEST(LinearRegression, HighVarianceExample)
    {
        // arrange
        valarray_fp x{ 1.0, 2.3, 3.1, 4.8, 5.6, 6.3 };
        valarray_fp y{ 2.6, 2.8, 3.1, 4.7, 5.1, 5.3 };

        // act
        auto regression = hdd::gamma::LinearRegression(x, y);

        // assert
        EXPECT_NEAR(0.5842, regression.gradient_, 1e-4);
        EXPECT_NEAR(1.6842, regression.intercept_, 1e-4);
        EXPECT_NEAR(0.2542, regression.standardError_, 1e-4);
    }
}
