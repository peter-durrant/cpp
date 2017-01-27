#pragma once

#include "data.h"

namespace hdd::gamma
{
    struct Node
    {
        std::vector<uint32_t> indexList_;
        uint32_t partitionKey_;
        uint32_t median_;
        Node* left_;
        Node* right_;

        Node(const valarray_uint& indexList); // terminal Node
        Node(uint32_t partitionKey, uint32_t median, Node* left, Node* right); // non-terminal Node

        bool Terminal() const;

        friend std::ostream& operator<<(std::ostream& os, const Node& node);
    };



    class KdTree
    {
    public:
        KdTree(const Data& source, const uint32_t bucketSize);
        virtual ~KdTree();

        const Node* Root() const;
        uint32_t Dimension() const;

        const IOVector& operator[](uint32_t index) const;

        friend std::ostream& operator<<(std::ostream& os, const KdTree& kdTree);

    private:
        const Data& data_;
        const uint32_t bucketsize_;
        valarray_uint indexList_;
        Node* tree_;

        Node* BuildTree(valarray_uint &indexList);
        uint32_t Median(const uint32_t partition_key, valarray_uint& indexList) const;
        valarray_fp CalculateSpread(const valarray_uint& indexList);
        uint32_t CalculateMaxSpread(const valarray_fp& spread);
        valarray_uint LeftSubSet(const uint32_t medianIndex, const valarray_uint& indexList);
        valarray_uint RightSubSet(const uint32_t medianIndex, const valarray_uint& indexList);
        Node* MakeNonTerminalNode(const uint32_t partitionKey, const uint32_t median, Node* left, Node* right);
        Node* MakeTerminalNode(const valarray_uint& indexList);
        void DeleteTree(Node** cn);
    };
}
