#pragma once

#include "../kdTree/kdtree.h"

struct near_points
{
    fp distance;
    vect_uint index_list;

    near_points(void);
    near_points(fp d, uint i);
    near_points(fp d, const vect_uint& il);

    friend std::ostream& operator<<(std::ostream& os, const near_points& n);
};

fp Distance(const IOVector& X1, const IOVector& X2, uint power = 2);
fp Distance(const valarray_fp& x1, const valarray_fp& x2, uint power = 2);
fp Distance(fp x1, fp x2, uint power = 2);
fp Dissim(fp x);

class Nearest
{
protected:
    const IOVector& query_point;
    const kdTree& kdtree;
    const uint pmax;
    std::vector<near_points*> near_neighbours;
    vect_uint zero_nn_list;
    fp furthest_distance;

    vect_fp upper_bound;
    vect_fp lower_bound;

    void Add_Point(uint index);
    void Pre_Search(void);
    void Search(const node* root);
    //   bool Ball_Within_Bounds(void);
    bool Bounds_Overlap_Ball(void);

public:
    Nearest(uint q, const kdTree& k, uint p);
    Nearest(const IOVector& q, const kdTree& k, uint p);
    ~Nearest(void);
    const std::vector<near_points*>& neighbours(void) const;
    const near_points& neighbours(uint p) const;
    friend std::ostream& operator<<(std::ostream& os, const Nearest& n);
};
