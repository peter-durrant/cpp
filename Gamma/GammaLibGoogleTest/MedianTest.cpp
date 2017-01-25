#include "stdafx.h"
#include "gtest/gtest.h"
#include <valarray>

namespace GammaLibTest
{
    TEST(Median, vector)
    {
        // arrange
        std::vector<int> v{ 5, 6, 4, 3, 2, 6, 7, 9, 3 };

        // act
        std::nth_element(v.begin(), v.begin() + v.size() / 2, v.end());

        // assert
        const int median = v[v.size() / 2];
        EXPECT_EQ(5, median);
    }

    struct Data
    {
        Data(int value, int associatedValue) : value_(value), associatedValue_(associatedValue) {}
        int value_;
        int associatedValue_;
    };

    TEST(Median, vector_struct)
    {
        // arrange
        auto lambda = [](Data& value1, Data& value2) { return value1.value_ < value2.value_; };
        std::vector<Data> v{ Data(5,1), Data(6,1), Data(4,1), Data(3,1), Data(2,1), Data(6,1), Data(7,1), Data(9,1), Data(3,1) };

        // act
        std::nth_element(v.begin(), v.begin() + v.size() / 2, v.end(), lambda);

        // assert
        const int median = v[v.size() / 2].value_;
        EXPECT_EQ(5, median);
    }

    TEST(Median, valarray_struct)
    {
        // arrange
        auto lambda = [](Data& value1, Data& value2) { return value1.value_ < value2.value_; };
        std::valarray<Data> v{ Data(5,1), Data(6,1), Data(4,1), Data(3,1), Data(2,1), Data(6,1), Data(7,1), Data(9,1), Data(3,1) };

        // act
        std::nth_element(std::begin(v), std::begin(v) + v.size() / 2, std::end(v), lambda);

        // assert
        const int median = v[v.size() / 2].value_;
        EXPECT_EQ(5, median);
    }
}
