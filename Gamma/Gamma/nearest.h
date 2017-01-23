#pragma once

#include "kdtree.h"

struct near_points
{
    double distance;
    std::vector<uint32_t> index_list;

    near_points();
    near_points(double d, uint32_t i);
    near_points(double d, const std::vector<uint32_t>& il);

    friend std::ostream& operator<<(std::ostream& os, const near_points& n);
};

double Distance(const IOVector& X1, const IOVector& X2, uint32_t power = 2);
double Distance(const valarray_fp& x1, const valarray_fp& x2, uint32_t power = 2);
double Distance(double x1, double x2, uint32_t power = 2);
double Dissim(double x);

class Nearest
{
public:
    Nearest(uint32_t q, const kdTree& k, uint32_t p);
    Nearest(const IOVector& q, const kdTree& k, uint32_t p);
    ~Nearest();
    const std::vector<near_points*>& neighbours() const;
    const near_points& neighbours(uint32_t p) const;
    friend std::ostream& operator<<(std::ostream& os, const Nearest& n);

protected:
    const IOVector& query_point;
    const kdTree& kdtree;
    const uint32_t pmax;
    std::vector<near_points*> near_neighbours;
    std::vector<uint32_t> zero_nn_list;
    double furthest_distance;

    std::vector<double> upper_bound;
    std::vector<double> lower_bound;

    void Add_Point(uint32_t index);
    void Pre_Search();
    void Search(const node* root);
    //   bool Ball_Within_Bounds();
    bool Bounds_Overlap_Ball();
};
