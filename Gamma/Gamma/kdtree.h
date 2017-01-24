#pragma once

#include "data.h"

namespace hdd::gamma
{
    struct Node
    {
        std::vector<uint32_t> index_list;
        uint32_t partition_key;
        uint32_t median;
        Node* left;
        Node* right;

        Node(valarray_uint& il); // terminal Node
        Node(uint32_t p, uint32_t m, Node* l, Node* r); // non-terminal Node

        bool Terminal() const;

        friend std::ostream& operator<<(std::ostream& os, const Node& n);
    };



    class KdTree
    {
    public:
        KdTree(const Data& source, uint32_t bsize);
        ~KdTree();
        const Node* Root() const;
        uint32_t Dimension() const;

        const IOVector& operator[](uint32_t index) const;

        friend std::ostream& operator<<(std::ostream& os, const KdTree& kd);

    private:
        const Data& data;
        const uint32_t bucketsize;
        valarray_uint index_list;
        Node* tree;

        Node* Build_Tree(valarray_uint &index_list);
        uint32_t Median(uint32_t partition_key, valarray_uint& index_list);
        void Calc_Spread(valarray_fp &spread_list, const valarray_uint& index_list);
        uint32_t Calc_Max_Spread(const valarray_fp &spread);
        void Swap(uint32_t& num1, uint32_t& num2);
        valarray_uint Left_Sub_Set(uint32_t median_index, const valarray_uint& index_list);
        valarray_uint Right_Sub_Set(uint32_t median_index, const valarray_uint& index_list);
        Node* Make_Non_Terminal_Node(uint32_t partition_key, uint32_t median, Node* left, Node* right);
        Node* Make_Terminal_Node(valarray_uint& index_list);
        void Delete_Tree(Node** cn);
    };
}
