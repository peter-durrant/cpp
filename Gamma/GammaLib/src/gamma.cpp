#include "gamma.h"

#include "nearest.h"
#include "kdtree.h"
#include "makestring.h"

using hdd::utility::MS;

const uint32_t moments_MIN = 2;
const uint32_t moments_MAX = 10;

namespace hdd::gamma
{
    std::ostream& operator<<(std::ostream& os, const hdd::gamma::Regression& r)
    {
        os << "     Intercept = " << r.intercept << std::endl;
        os << "      Gradient = " << r.gradient << std::endl;
        os << "Standard error = " << r.standard_error << std::endl;
        return os;
    }

    Regression Linear_Regression(const valarray_fp& x, const valarray_fp& y)
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
        const uint32_t num_near = x.size();
        const double num_near_fp = num_near;

        for (uint32_t i = 1; i <= num_near; ++i)
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

        sx = sqrt(Qx / num_near_fp);
        sy = sqrt(Qy / num_near_fp);

        Regression regression;
        regression.gradient = Qxy / (num_near_fp * sx * sx);
        regression.intercept = ybar - (regression.gradient * xbar);

        if (num_near == 2)
        {
            regression.standard_error = 0;
        }
        else
        {
            regression.standard_error = sqrt((sy*sy) - (regression.gradient * regression.gradient * sx * sx));
        }

        return regression;
    }

    Gamma::Gamma(const Data& d, uint32_t p, uint32_t num_m) :
        data(d), pmax(p), high_moments(num_m)
    {
        if (high_moments > moments_MAX || high_moments < moments_MIN)
        {
            throw std::runtime_error(MS() << "Invalid number of moments " << high_moments);
        }

        const uint32_t num_moments = high_moments - moments_MIN + 1;
        delta.resize(num_moments);
        gamma.resize(num_moments);
        r.resize(num_moments);
        moments.resize(num_moments);

        for (uint32_t i = 0; i < num_moments; ++i)
        {
            //      delta[i] = valarray_fp(0, pmax);
            delta[i].resize(pmax);
            gamma[i].resize(data.Outputs());
            //   	for (uint32_t j = 0; j < data.Outputs(); j++) gamma[i][j] = valarray_fp(0, pmax);
            for (uint32_t j = 0; j < data.Outputs(); j++)
            {
                gamma[i][j].resize(pmax);
            }
            r[i].resize(data.Outputs());
            //      moments[i] = valarray_fp(0, data.Outputs());
            moments[i].resize(data.Outputs());
        }

        KdTree kdtree(data, 16);

        for (uint32_t i = 0; i < data.Size(); ++i)
        {
            Nearest nn(data[i], kdtree, pmax);
            const std::vector<NearPoints*>& neighbours = nn.neighbours();

            for (uint32_t m = 0; m < num_moments; ++m)
            {
                if ((m + moments_MIN) % 2 != 0) continue;

                for (uint32_t j = 0; j < pmax; ++j)
                {
                    //				original delta = (x_i - x_N[i,p])^k
                    //	   		delta[m][j] += Dissim(Raise(neighbours[j]->distance,m+moments_MIN)); // sqrt the squared distances

                    delta[m][j] += neighbours[j]->distance_;
                    //            else delta[m][j] += Dissim(neighbours[j]->distance);
                    const std::vector<uint32_t>& nn_index_list = neighbours[j]->indexList_;

                    for (uint32_t k = 0; k < data.Outputs(); ++k)
                    {
                        valarray_fp local_gamma(0.0, pmax);
                        for (uint32_t l = 0; l < nn_index_list.size(); l++)
                        {
                            const valarray_fp& output1 = data[nn_index_list[l]].Output_Vector();
                            const valarray_fp& output2 = data[i].Output_Vector();
                            local_gamma[j] += Distance(output1[k], output2[k], m + moments_MIN);
                        }
                        gamma[m][k] += local_gamma / (double)nn_index_list.size();
                    }
                }

            }
        }

        for (uint32_t m = 0; m < num_moments; ++m)
        {
            if ((m + moments_MIN) % 2 != 0)
            {
                continue;
            }
            delta[m] /= data.Size();

            for (uint32_t i = 0; i < data.Outputs(); ++i)
            {
                gamma[m][i] /= data.Size();
            }

            for (uint32_t i = 0; i < data.Outputs(); ++i)
            {
                r[m][i] = Linear_Regression(delta[m], gamma[m][i]);
            }
        }

        for (uint32_t m = 0; m < num_moments; ++m)
        {
            for (uint32_t i = 0; i < data.Outputs(); ++i)
            {
                switch (m + moments_MIN)
                {
                default:
                    break;

                case 2:
                    moments[m][i] = r[m][i].intercept / 2;
                    break;

                case 4:
                    moments[m][i] = (r[m][i].intercept - 6 * pow(moments[m - 2][i], 2)) / 2;
                    break;

                case 6:
                    moments[m][i] = (r[m][i].intercept - 30 * moments[m - 4][i] * moments[m - 2][i]) / 2;
                    break;

                case 8:
                    moments[m][i] = (r[m][i].intercept - 70 * pow(moments[m - 4][i], 2) - 56 * moments[m - 6][i] * moments[m - 2][i]) / 2;
                    break;

                case 10:
                    moments[m][i] = (r[m][i].intercept - 420 * moments[m - 6][i] * moments[m - 4][i] - 90 * moments[m - 8][i] * moments[m - 2][i]) / 2;
                    break;
                }
            }
        }
    }

    /*
    ostream& operator<<(ostream& os, const Gamma& g)
    {
       const uint32_t num_moments = g.high_moments - moments_MIN + 1;
       for (uint32_t m = 0; m < num_moments; m++) {
          if ((m + moments_MIN) % 2 == 0) {
             os << "moment " << (m + moments_MIN);
             os << endl;
    //      	for (uint32_t i = 0; i < g.pmax; i++) {
    //	      	os << "delta[" << (i+1) << "] = " << g.delta[m][i] << endl;
    //		      for (uint32_t j = 0; j < g.data.Outputs(); j++) {
    //      			os << "output[" << (j+1) << "] -> " << "gamma[" << (i+1) << "] = " << g.gamma[m][j][i] << endl;
    //            }
    //         }
             for (uint32_t i = 0; i < g.data.Outputs(); i++) {
                os << "Output: " << i;
                if (i < g.data.Outputs()-1) os << ", ";
                else os << endl;
             }

             os << "Moment: ";
             for (uint32_t i = 0; i < g.data.Outputs(); i++) {
                os << g.moments[m][i];
                if (i < g.data.Outputs()-1) os << ", ";
                else os << endl;
             }

             os << "Intercept: ";
             for (uint32_t i = 0; i < g.data.Outputs(); i++) {
                os << g.r[m][i].intercept;
                if (i < g.data.Outputs()-1) os << ", ";
                else os << endl;
             }

             os << "Gradient: ";
             for (uint32_t i = 0; i < g.data.Outputs(); i++) {
                os << g.r[m][i].gradient;
                if (i < g.data.Outputs()-1) os << ", ";
                else os << endl;
             }

             os << "Standard error: ";
             for (uint32_t i = 0; i < g.data.Outputs(); i++) {
                os << g.r[m][i].standard_error;
                if (i < g.data.Outputs()-1) os << ", ";
                else os << endl;
             }
             os << endl;
    //         os << g.r[m][i] << endl;
          }
       }
       return os;
    }
    */

    std::ostream& operator<<(std::ostream& os, const Gamma& g)
    {
        //   const uint32_t num_moments = g.high_moments - moments_MIN + 1;

        //   for (uint32_t i = 0; i < g.data.Outputs(); i++)
        //      os << "num points, G2, G4, G6, G8, G10, M2, M4, M6, M8, M10,";
        //   os << endl;

        for (uint32_t i = 0; i < g.data.Outputs(); ++i)
        {
            os << g.data.Size() << ", ";

            if (g.high_moments >= 2) os << g.r[0][i].intercept << ", ";
            if (g.high_moments >= 4) os << g.r[2][i].intercept << ", ";
            if (g.high_moments >= 6) os << g.r[4][i].intercept << ", ";
            if (g.high_moments >= 8) os << g.r[6][i].intercept << ", ";
            if (g.high_moments >= 10) os << g.r[8][i].intercept << ", ";

            if (g.high_moments >= 2) os << g.moments[0][i] << ", ";
            if (g.high_moments >= 4) os << g.moments[2][i] << ", ";
            if (g.high_moments >= 6) os << g.moments[4][i] << ", ";
            if (g.high_moments >= 8) os << g.moments[6][i] << ", ";
            if (g.high_moments >= 10) os << g.moments[8][i] << ", ";

            if (g.high_moments >= 2) os << g.r[0][i].gradient << ", ";
            if (g.high_moments >= 4) os << g.r[2][i].gradient << ", ";
            if (g.high_moments >= 6) os << g.r[4][i].gradient << ", ";
            if (g.high_moments >= 8) os << g.r[6][i].gradient << ", ";
            if (g.high_moments >= 10) os << g.r[8][i].gradient << ", ";

            if (g.high_moments >= 2) os << g.r[0][i].standard_error << ", ";
            if (g.high_moments >= 4) os << g.r[2][i].standard_error << ", ";
            if (g.high_moments >= 6) os << g.r[4][i].standard_error << ", ";
            if (g.high_moments >= 8) os << g.r[6][i].standard_error << ", ";
            if (g.high_moments >= 10) os << g.r[8][i].standard_error << ", ";
        }
        return os;
    }
}