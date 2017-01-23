#include "stdafx.h"
#include "gtest/gtest.h"
#include "../Gamma/universal.h"

namespace GammaLibTest
{
    TEST(Pow, 20)
    {
        double total = 0;
        for (int i = 0; i < 2000000; ++i)
        {
            double value = pow(12.45, 25.0);
            total += value;
        }
        EXPECT_GT(total, 0);
    }
}
