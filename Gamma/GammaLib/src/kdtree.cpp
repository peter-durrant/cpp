#include "kdtree.h"

#include <sstream>
#include <algorithm>

namespace hdd::gamma
{
    Node::Node(const valarray_uint& indexList) :
        indexList_(indexList.size()), partitionKey_(0), median_(0), left_(0), right_(0)
    {
        for (uint32_t i = 0; i < indexList_.size(); ++i)
        {
            indexList_[i] = indexList[i];
        }
    }

    Node::Node(const uint32_t partitionKey, const uint32_t median, Node* left, Node* right) :
        indexList_(0), partitionKey_(partitionKey), median_(median), left_(left), right_(right)
    {}

    bool Node::Terminal() const
    {
        return !left_;
    }


    KdTree::KdTree(const Data& source, const uint32_t bucketSize) :
        data_(source), bucketsize_(bucketSize), indexList_(data_.Size()), tree_(0)
    {
        for (uint32_t i = 0; i < indexList_.size(); ++i)
        {
            indexList_[i] = i;
        }
        tree_ = BuildTree(indexList_);
    }

    KdTree::~KdTree()
    {
        DeleteTree(&tree_);
    }

    void KdTree::DeleteTree(Node** cn)
    {
        Node* current_node = *cn;

        if (current_node->left_)
        {
            DeleteTree(&current_node->left_);
        }

        if (current_node->right_)
        {
            DeleteTree(&current_node->right_);
        }

        if (current_node)
        {
            delete current_node;
        }

        *cn = 0;
    }

    Node* KdTree::BuildTree(valarray_uint& indexList)
    {
        if (indexList.size() <= bucketsize_)
        {
            return MakeTerminalNode(indexList);
        }

        const valarray_fp spreadList = CalculateSpread(indexList);
        const uint32_t partitionKey = CalculateMaxSpread(spreadList);
        const uint32_t medianIndex = Median(partitionKey, indexList);

        valarray_uint leftss = LeftSubSet(medianIndex, indexList);
        Node* left = BuildTree(leftss);
        valarray_uint rightss = RightSubSet(medianIndex, indexList);
        Node* right = BuildTree(rightss);
        return MakeNonTerminalNode(partitionKey, indexList[medianIndex], left, right);

        /*return Make_Non_Terminal_Node(partition_key, index_list[median_index], Build_Tree(Left_Sub_Set(median_index, index_list)), Build_Tree(Right_Sub_Set(median_index, index_list))); */
    }

    uint32_t KdTree::Median(const uint32_t partitionKey, valarray_uint& indexList) const
    {
        auto lambda = [=](const uint32_t index1, const uint32_t index2) { return data_[index1].Input_Vector(partitionKey) < data_[index2].Input_Vector(partitionKey); };
        std::nth_element(std::begin(indexList), std::begin(indexList) + indexList.size() / 2, std::end(indexList), lambda);
        auto medianIndex = indexList.size() / 2;
        return medianIndex;
    }

    valarray_fp KdTree::CalculateSpread(const valarray_uint& indexList)
    {
        valarray_fp spreadList(0.0, data_.Inputs()); // initialise spread to zero
        valarray_fp min(std::numeric_limits<double>::max(), spreadList.size());
        valarray_fp max(std::numeric_limits<double>::min(), spreadList.size());

        for (uint32_t i = 0; i < indexList.size(); ++i)
        {
            for (uint32_t j = 0; j < data_.Inputs(); ++j)
            {
                if (data_[indexList[i]].Input_Vector(j) < min[j])
                {
                    min[j] = data_[indexList[i]].Input_Vector(j);
                }
                if (data_[indexList[i]].Input_Vector(j) > max[j])
                {
                    max[j] = data_[indexList[i]].Input_Vector(j);
                }
            }
        }
        spreadList = max - min;
        return spreadList;
    }

    uint32_t KdTree::CalculateMaxSpread(const valarray_fp &spread)
    {
        double maxspread = 0;
        uint32_t maxspreadIndex = 0;
        for (uint32_t i = 0; i < spread.size(); ++i)
        {
            if (spread[i] > maxspread)
            {
                maxspread = spread[i];
                maxspreadIndex = i;
            }
        }
        return maxspreadIndex;
    }

    valarray_uint KdTree::LeftSubSet(const uint32_t medianIndex, const valarray_uint& indexList)
    {
        return indexList[std::slice(0, medianIndex, 1)];
    }

    valarray_uint KdTree::RightSubSet(const uint32_t medianIndex, const valarray_uint& indexList)
    {
        return indexList[std::slice(medianIndex, indexList.size() - medianIndex, 1)];
    }

    Node* KdTree::MakeNonTerminalNode(const uint32_t partitionKey, uint32_t median, /*const*/ Node* left, /*const*/ Node* right)
    {
        return new Node(partitionKey, median, left, right);
    }

    Node* KdTree::MakeTerminalNode(const valarray_uint& indexList)
    {
        return new Node(indexList);
    }

    const Node* KdTree::Root() const
    {
        return tree_;
    }

    uint32_t KdTree::Dimension() const
    {
        return data_.Inputs();
    }

    const IOVector& KdTree::operator[](uint32_t index) const
    {
        return data_[index];
    }

    std::ostream& operator<<(std::ostream& os, const Node& node)
    {
        if (node.left_) // recursive
        {
            os << *(node.left_);
        }

        if (node.right_) // recursive
        {
            os << *(node.right_);
        }

        if (!node.left_ || !node.right_)
        {
            os << "List size = " << node.indexList_.size() << std::endl;
            for (unsigned int i = 0; i < node.indexList_.size(); ++i)
            {
                os << node.indexList_[i] << std::endl;
            }
        }

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const KdTree& kdTree)
    {
        os << *(kdTree.Root()) << std::endl;
        return os;
    }
}