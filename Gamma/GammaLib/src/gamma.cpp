#include "gamma.h"

#include "nearest.h"
#include "kdtree.h"
#include "makestring.h"

using hdd::utility::MS;

namespace hdd::gamma
{
    Gamma::Gamma(const Data& data, uint32_t pmax, uint32_t numMoments) :
        data_(data), pmax_(pmax), highMoments_(numMoments)
    {
        if (highMoments_ > MomentsMax() || highMoments_ < MomentsMin())
        {
            throw std::runtime_error(MS() << "Invalid number of moments " << highMoments_);
        }

        const uint32_t num_moments = highMoments_ - MomentsMin() + 1;
        delta_.resize(num_moments);
        gamma_.resize(num_moments);
        r_.resize(num_moments);
        moments_.resize(num_moments);

        for (uint32_t i = 0; i < num_moments; ++i)
        {
            delta_[i].resize(pmax_);
            gamma_[i].resize(data_.Outputs());
            for (uint32_t j = 0; j < data_.Outputs(); ++j)
            {
                gamma_[i][j].resize(pmax_);
            }
            r_[i].resize(data_.Outputs());
            moments_[i].resize(data_.Outputs());
        }

        KdTree kdtree(data_, 16);

        for (uint32_t i = 0; i < data_.Size(); ++i)
        {
            Nearest nn(data_[i], kdtree, pmax_);
            const std::vector<NearPoints*>& neighbours = nn.neighbours();

            for (uint32_t m = 0; m < num_moments; ++m)
            {
                if ((m + MomentsMin()) % 2 != 0) continue;

                for (uint32_t j = 0; j < pmax_; ++j)
                {
                    delta_[m][j] += neighbours[j]->distance_;
                    const std::vector<uint32_t>& nn_index_list = neighbours[j]->indexList_;

                    for (uint32_t k = 0; k < data_.Outputs(); ++k)
                    {
                        valarray_fp local_gamma(0.0, pmax_);
                        for (uint32_t l = 0; l < nn_index_list.size(); ++l)
                        {
                            const valarray_fp& output1 = data_[nn_index_list[l]].OutputVector();
                            const valarray_fp& output2 = data_[i].OutputVector();
                            local_gamma[j] += Distance(output1[k], output2[k], m + MomentsMin());
                        }
                        gamma_[m][k] += local_gamma / (double)nn_index_list.size();
                    }
                }

            }
        }

        for (uint32_t m = 0; m < num_moments; ++m)
        {
            if ((m + MomentsMin()) % 2 != 0)
            {
                continue;
            }
            delta_[m] /= data_.Size();

            for (uint32_t i = 0; i < data_.Outputs(); ++i)
            {
                gamma_[m][i] /= data_.Size();
            }

            for (uint32_t i = 0; i < data_.Outputs(); ++i)
            {
                r_[m][i] = LinearRegression(delta_[m], gamma_[m][i]);
            }
        }

        for (uint32_t m = 0; m < num_moments; ++m)
        {
            for (uint32_t i = 0; i < data_.Outputs(); ++i)
            {
                switch (m + MomentsMin())
                {
                default:
                    break;

                case 2:
                    moments_[m][i] = r_[m][i].intercept_ / 2;
                    break;

                case 4:
                    moments_[m][i] = (r_[m][i].intercept_ - 6 * pow(moments_[m - 2][i], 2)) / 2;
                    break;

                case 6:
                    moments_[m][i] = (r_[m][i].intercept_ - 30 * moments_[m - 4][i] * moments_[m - 2][i]) / 2;
                    break;

                case 8:
                    moments_[m][i] = (r_[m][i].intercept_ - 70 * pow(moments_[m - 4][i], 2) - 56 * moments_[m - 6][i] * moments_[m - 2][i]) / 2;
                    break;

                case 10:
                    moments_[m][i] = (r_[m][i].intercept_ - 420 * moments_[m - 6][i] * moments_[m - 4][i] - 90 * moments_[m - 8][i] * moments_[m - 2][i]) / 2;
                    break;
                }
            }
        }
    }

    std::ostream& operator<<(std::ostream& os, const Gamma& g)
    {
        for (uint32_t i = 0; i < g.data_.Outputs(); ++i)
        {
            os << g.data_.Size() << ", ";

            if (g.highMoments_ >= 2) os << g.r_[0][i].intercept_ << ", ";
            if (g.highMoments_ >= 4) os << g.r_[2][i].intercept_ << ", ";
            if (g.highMoments_ >= 6) os << g.r_[4][i].intercept_ << ", ";
            if (g.highMoments_ >= 8) os << g.r_[6][i].intercept_ << ", ";
            if (g.highMoments_ >= 10) os << g.r_[8][i].intercept_ << ", ";

            if (g.highMoments_ >= 2) os << g.moments_[0][i] << ", ";
            if (g.highMoments_ >= 4) os << g.moments_[2][i] << ", ";
            if (g.highMoments_ >= 6) os << g.moments_[4][i] << ", ";
            if (g.highMoments_ >= 8) os << g.moments_[6][i] << ", ";
            if (g.highMoments_ >= 10) os << g.moments_[8][i] << ", ";

            if (g.highMoments_ >= 2) os << g.r_[0][i].gradient_ << ", ";
            if (g.highMoments_ >= 4) os << g.r_[2][i].gradient_ << ", ";
            if (g.highMoments_ >= 6) os << g.r_[4][i].gradient_ << ", ";
            if (g.highMoments_ >= 8) os << g.r_[6][i].gradient_ << ", ";
            if (g.highMoments_ >= 10) os << g.r_[8][i].gradient_ << ", ";

            if (g.highMoments_ >= 2) os << g.r_[0][i].standardError_ << ", ";
            if (g.highMoments_ >= 4) os << g.r_[2][i].standardError_ << ", ";
            if (g.highMoments_ >= 6) os << g.r_[4][i].standardError_ << ", ";
            if (g.highMoments_ >= 8) os << g.r_[6][i].standardError_ << ", ";
            if (g.highMoments_ >= 10) os << g.r_[8][i].standardError_ << ", ";
        }
        return os;
    }
}