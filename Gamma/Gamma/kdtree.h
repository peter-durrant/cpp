#pragma once

#include "data.h"

struct node
{
    vect_uint index_list;
    uint partition_key;
    uint median;
    /*const*/ node* left;
    /*const*/ node* right;

    node(valarray_uint& il);	// terminal node
    node(uint p, uint m, /*const*/ node* l, /*const*/ node* r); // non-terminal node

    bool Terminal() const;

    friend std::ostream& operator<<(std::ostream& os, const node& n);
};



class kdTree
{
private:
    const Data& data;
    const uint bucketsize;
    valarray_uint index_list;
    node* tree;

    node* Build_Tree(valarray_uint &index_list);
    uint Median(uint partition_key, valarray_uint& index_list);
    void Calc_Spread(valarray_fp &spread_list, const valarray_uint& index_list);
    uint Calc_Max_Spread(const valarray_fp &spread);
    void Swap(uint& num1, uint& num2);
    valarray_uint Left_Sub_Set(uint median_index, const valarray_uint& index_list);
    valarray_uint Right_Sub_Set(uint median_index, const valarray_uint& index_list);
    node* Make_Non_Terminal_Node(uint partition_key, uint median, node* left, node* right);
    node* Make_Terminal_Node(valarray_uint& index_list);
    void Delete_Tree(node** cn);


public:
    kdTree(const Data& source, uint bsize);
    ~kdTree();
    const node* Root() const;
    uint Dimension() const;

    const IOVector& operator[](uint index) const;

    friend std::ostream& operator<<(std::ostream& os, const kdTree& kd);
};
