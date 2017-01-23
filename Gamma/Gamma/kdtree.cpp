#include "kdtree.h"

#include <sstream>

namespace hdd::gamma
{
    Node::Node(valarray_uint& il) :
        index_list(il.size()), partition_key(0), median(0), left(0), right(0)
    {
        for (uint32_t i = 0; i < index_list.size(); ++i)
        {
            index_list[i] = il[i];
        }
    }

    Node::Node(uint32_t p, uint32_t m, Node* l, Node* r) :
        index_list(0), partition_key(p), median(m), left(l), right(r)
    {}

    bool Node::Terminal() const
    {
        return !left;
    }


    KdTree::KdTree(const Data& source, uint32_t bsize) :
        data(source), bucketsize(bsize), index_list(data.Size()), tree(0)
    {
        for (uint32_t i = 0; i < index_list.size(); ++i)
        {
            index_list[i] = i;
        }
        tree = Build_Tree(index_list);
    }

    KdTree::~KdTree()
    {
        Delete_Tree(&tree);
    }

    void KdTree::Delete_Tree(Node** cn)
    {
        Node* current_node = *cn;

        if (current_node->left)
        {
            Delete_Tree(&current_node->left);
        }

        if (current_node->right)
        {
            Delete_Tree(&current_node->right);
        }

        if (current_node)
        {
            delete current_node;
        }

        *cn = 0;
    }

    Node* KdTree::Build_Tree(valarray_uint& index_list)
    {
        if (index_list.size() <= bucketsize)
        {
            return Make_Terminal_Node(index_list);
        }

        valarray_fp spread_list(0.0, data.Inputs()); // initialise spread to zero
        Calc_Spread(spread_list, index_list);
        const uint32_t partition_key = Calc_Max_Spread(spread_list);
        uint32_t median_index = Median(partition_key, index_list);

        valarray_uint leftss = Left_Sub_Set(median_index, index_list);
        Node* left = Build_Tree(leftss);
        valarray_uint rightss = Right_Sub_Set(median_index, index_list);
        Node* right = Build_Tree(rightss);
        return Make_Non_Terminal_Node(partition_key, index_list[median_index], left, right);

        /*return Make_Non_Terminal_Node(partition_key, index_list[median_index], Build_Tree(Left_Sub_Set(median_index, index_list)), Build_Tree(Right_Sub_Set(median_index, index_list))); */
    }

    uint32_t KdTree::Median(uint32_t partition_key, valarray_uint& index_list)
    {
        uint32_t median_index = index_list.size() / 2;

        uint32_t l = 1;
        uint32_t ir = index_list.size();
        uint32_t mid;

        for (;;)
        {
            if (ir <= l + 1)
            {
                if (ir == l + 1 && data[index_list[ir - 1]].Input_Vector(partition_key) < data[index_list[l - 1]].Input_Vector(partition_key))
                {
                    Swap(index_list[l - 1], index_list[ir - 1]);
                }
                return median_index - 1;
            }
            else
            {
                mid = (l + ir) >> 1;
                Swap(index_list[mid - 1], index_list[l]);
                if (data[index_list[l - 1]].Input_Vector(partition_key) > data[index_list[ir - 1]].Input_Vector(partition_key))
                {
                    Swap(index_list[l - 1], index_list[ir - 1]);
                }
                if (data[index_list[l]].Input_Vector(partition_key) > data[index_list[ir - 1]].Input_Vector(partition_key))
                {
                    Swap(index_list[l], index_list[ir - 1]);
                }
                if (data[index_list[l - 1]].Input_Vector(partition_key) > data[index_list[l]].Input_Vector(partition_key))
                {
                    Swap(index_list[l - 1], index_list[l]);
                }

                uint32_t i = l + 1;
                uint32_t j = ir;
                uint32_t a = l + 1;
                uint32_t temp_a = index_list[a - 1];
                for (;;)
                {
                    do
                    {
                        i++;
                    } while (data[index_list[i - 1]].Input_Vector(partition_key) < data[index_list[a - 1]].Input_Vector(partition_key));

                    do
                    {
                        j--;
                    } while (data[index_list[j - 1]].Input_Vector(partition_key) > data[index_list[a - 1]].Input_Vector(partition_key));

                    if (j < i)
                    {
                        break;
                    }

                    Swap(index_list[i - 1], index_list[j - 1]);
                }

                index_list[l] = index_list[j - 1];
                index_list[j - 1] = temp_a;
                if (j >= median_index)
                {
                    ir = j - 1;
                }
                if (j <= median_index)
                {
                    l = i;
                }
            }
        }
    }

    void KdTree::Swap(uint32_t& num1, uint32_t& num2)
    {
        uint32_t temp = num1;
        num1 = num2;
        num2 = temp;
    }

    void KdTree::Calc_Spread(valarray_fp &spread_list, const valarray_uint& index_list)
    {
        valarray_fp min(std::numeric_limits<double>::max(), spread_list.size());
        valarray_fp max(std::numeric_limits<double>::min(), spread_list.size());

        for (uint32_t i = 0; i < index_list.size(); ++i)
        {
            for (uint32_t j = 0; j < data.Inputs(); ++j)
            {
                if (data[index_list[i]].Input_Vector(j) < min[j])
                {
                    min[j] = data[index_list[i]].Input_Vector(j);
                }
                if (data[index_list[i]].Input_Vector(j) > max[j])
                {
                    max[j] = data[index_list[i]].Input_Vector(j);
                }
            }
        }
        spread_list = max - min;
    }

    uint32_t KdTree::Calc_Max_Spread(const valarray_fp &spread)
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

    valarray_uint KdTree::Left_Sub_Set(uint32_t median_index, const valarray_uint& index_list)
    {
        return index_list[std::slice(0, median_index, 1)];
    }

    valarray_uint KdTree::Right_Sub_Set(uint32_t median_index, const valarray_uint& index_list)
    {
        return index_list[std::slice(median_index, index_list.size() - median_index, 1)];
    }

    Node* KdTree::Make_Non_Terminal_Node(uint32_t partition_key, uint32_t median, /*const*/ Node* left, /*const*/ Node* right)
    {
        return new Node(partition_key, median, left, right);
    }

    Node* KdTree::Make_Terminal_Node(valarray_uint& index_list)
    {
        return new Node(index_list);
    }

    const Node* KdTree::Root() const
    {
        return tree;
    }

    uint32_t KdTree::Dimension() const
    {
        return data.Inputs();
    }

    const IOVector& KdTree :: operator[](uint32_t index) const
    {
        return data[index];
    }

    std::ostream& operator<<(std::ostream& os, const Node& n)
    {
        if (n.left) // recursive
        {
            os << *(n.left);
        }

        if (n.right) // recursive
        {
            os << *(n.right);
        }

        if (!n.left || !n.right)
        {
            os << "List size = " << n.index_list.size() << std::endl;
            for (unsigned int i = 0; i < n.index_list.size(); ++i)
            {
                os << n.index_list[i] << std::endl;
            }
        }

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const KdTree& kd)
    {
        os << *(kd.Root()) << std::endl;
        return os;
    }
}