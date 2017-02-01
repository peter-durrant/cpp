#pragma once

#include "data.h"
#include "linearregression.h"

namespace hdd::gamma
{
    class Gamma
    {
    public:
        Gamma(const Data& data, uint32_t pMax, uint32_t numMoments = MomentsMin());
        friend std::ostream& operator<<(std::ostream& os, const Gamma& g);
        static constexpr uint32_t MomentsMin() { return 2; }
        static constexpr uint32_t MomentsMax() { return 10; }

    private:
        const Data& data_;
        const uint32_t pmax_;
        const uint32_t highMoments_;

        std::vector<valarray_fp> delta_;
        std::vector<std::vector<valarray_fp>> gamma_;
        std::vector<std::vector<Regression>> r_;
        std::vector<valarray_fp> moments_;
    };
}