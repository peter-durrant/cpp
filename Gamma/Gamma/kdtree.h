#pragma once

#include "data.h"

struct node
{
    std::vector<uint32_t> index_list;
    uint32_t partition_key;
    uint32_t median;
    /*const*/ node* left;
    /*const*/ node* right;

    node(valarray_uint& il);	// terminal node
    node(uint32_t p, uint32_t m, /*const*/ node* l, /*const*/ node* r); // non-terminal node

    bool Terminal() const;

    friend std::ostream& operator<<(std::ostream& os, const node& n);
};



class kdTree
{
public:
    kdTree(const Data& source, uint32_t bsize);
    ~kdTree();
    const node* Root() const;
    uint32_t Dimension() const;

    const IOVector& operator[](uint32_t index) const;

    friend std::ostream& operator<<(std::ostream& os, const kdTree& kd);

private:
    const Data& data;
    const uint32_t bucketsize;
    valarray_uint index_list;
    node* tree;

    node* Build_Tree(valarray_uint &index_list);
    uint32_t Median(uint32_t partition_key, valarray_uint& index_list);
    void Calc_Spread(valarray_fp &spread_list, const valarray_uint& index_list);
    uint32_t Calc_Max_Spread(const valarray_fp &spread);
    void Swap(uint32_t& num1, uint32_t& num2);
    valarray_uint Left_Sub_Set(uint32_t median_index, const valarray_uint& index_list);
    valarray_uint Right_Sub_Set(uint32_t median_index, const valarray_uint& index_list);
    node* Make_Non_Terminal_Node(uint32_t partition_key, uint32_t median, node* left, node* right);
    node* Make_Terminal_Node(valarray_uint& index_list);
    void Delete_Tree(node** cn);
};
