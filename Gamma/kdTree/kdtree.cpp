#include "kdtree.h"

node::node(valarray_uint& il)
    : index_list(il.size()), partition_key(0), median(0), left(0), right(0)
{
    for (uint i = 0; i < index_list.size(); i++)
        index_list[i] = il[i];
}

node::node(uint p, uint m, node* l, node* r)
    : index_list(0), partition_key(p), median(m), left(l), right(r)
{}

bool node::Terminal() const
{
    return !left;
}

std::ostream& operator<<(std::ostream& os, const node& n)
{
    if (n.left) os << *(n.left); 		// recursive
    if (n.right) os << *(n.right); 	// recursive

    if (!n.left || !n.right) {
        os << "List size = " << n.index_list.size() << std::endl;
        for (unsigned int i = 0; i < n.index_list.size(); i++)
            os << n.index_list[i] << std::endl;
    }

    return os;
}

kdTree::kdTree(const Data& source, uint bsize)
    : data(source), bucketsize(bsize), index_list(data.Size()), tree(0)
{
    for (uint i = 0; i < index_list.size(); i++)
        index_list[i] = i;
    tree = Build_Tree(index_list);
}

kdTree :: ~kdTree()
{
    Delete_Tree(&tree);
}

void kdTree::Delete_Tree(node** cn)
{
    node* current_node = *cn;
    if (current_node->left) Delete_Tree(&current_node->left);
    if (current_node->right) Delete_Tree(&current_node->right);

    if (current_node) delete current_node;
    *cn = 0;
}

node* kdTree::Build_Tree(valarray_uint& index_list)
{
    if (index_list.size() <= bucketsize)
        return Make_Terminal_Node(index_list);

    valarray_fp spread_list(fp_ZERO, data.Inputs()); // initialise spread to zero
    Calc_Spread(spread_list, index_list);
    const uint partition_key = Calc_Max_Spread(spread_list);
    uint median_index = Median(partition_key, index_list);

    valarray_uint leftss = Left_Sub_Set(median_index, index_list);
    node* left = Build_Tree(leftss);
    valarray_uint rightss = Right_Sub_Set(median_index, index_list);
    node* right = Build_Tree(rightss);
    return Make_Non_Terminal_Node(partition_key, index_list[median_index], left, right);

    /*return Make_Non_Terminal_Node(partition_key, index_list[median_index], Build_Tree(Left_Sub_Set(median_index, index_list)), Build_Tree(Right_Sub_Set(median_index, index_list))); */
}

uint kdTree::Median(uint partition_key, valarray_uint& index_list)
{
    uint median_index = index_list.size() / 2;

    uint l = 1;
    uint ir = index_list.size();
    uint mid;

    for (;;) {
        if (ir <= l + 1) {
            if (ir == l + 1 && data[index_list[ir - 1]].Input_Vector(partition_key) < data[index_list[l - 1]].Input_Vector(partition_key)) {
                Swap(index_list[l - 1], index_list[ir - 1]);
            }
            return median_index - 1;
        }
        else {
            mid = (l + ir) >> 1;
            Swap(index_list[mid - 1], index_list[l]);
            if (data[index_list[l - 1]].Input_Vector(partition_key) > data[index_list[ir - 1]].Input_Vector(partition_key)) {
                Swap(index_list[l - 1], index_list[ir - 1]);
            }
            if (data[index_list[l]].Input_Vector(partition_key) > data[index_list[ir - 1]].Input_Vector(partition_key)) {
                Swap(index_list[l], index_list[ir - 1]);
            }
            if (data[index_list[l - 1]].Input_Vector(partition_key) > data[index_list[l]].Input_Vector(partition_key)) {
                Swap(index_list[l - 1], index_list[l]);
            }

            uint i = l + 1;
            uint j = ir;
            uint a = l + 1;
            uint temp_a = index_list[a - 1];
            for (;;) {
                do i++; while (data[index_list[i - 1]].Input_Vector(partition_key) < data[index_list[a - 1]].Input_Vector(partition_key));
                do j--; while (data[index_list[j - 1]].Input_Vector(partition_key) > data[index_list[a - 1]].Input_Vector(partition_key));
                if (j < i) break;
                Swap(index_list[i - 1], index_list[j - 1]);
            }

            index_list[l] = index_list[j - 1];
            index_list[j - 1] = temp_a;
            if (j >= median_index) ir = j - 1;
            if (j <= median_index) l = i;
        }
    }
}

void kdTree::Swap(uint& num1, uint& num2) {
    uint temp = num1;
    num1 = num2;
    num2 = temp;
}

void kdTree::Calc_Spread(valarray_fp &spread_list, const valarray_uint& index_list)
{
    valarray_fp min(fp_MAX, spread_list.size());
    valarray_fp max(fp_MIN, spread_list.size());

    for (uint i = 0; i < index_list.size(); i++)
        for (uint j = 0; j < data.Inputs(); j++) {
            if (data[index_list[i]].Input_Vector(j) < min[j])
                min[j] = data[index_list[i]].Input_Vector(j);
            if (data[index_list[i]].Input_Vector(j) > max[j])
                max[j] = data[index_list[i]].Input_Vector(j);
        }

    spread_list = max - min;
}

uint kdTree::Calc_Max_Spread(const valarray_fp &spread)
{
    fp maxspread = fp_ZERO;
    uint maxspreadIndex = 0;
    for (uint i = 0; i < spread.size(); i++) {
        if (spread[i] > maxspread) {
            maxspread = spread[i];
            maxspreadIndex = i;
        }
    }
    return maxspreadIndex;
}

valarray_uint kdTree::Left_Sub_Set(uint median_index, const valarray_uint& index_list)
{
    return index_list[std::slice(0, median_index, 1)];
}

valarray_uint kdTree::Right_Sub_Set(uint median_index, const valarray_uint& index_list)
{
    return index_list[std::slice(median_index, index_list.size() - median_index, 1)];
}

node* kdTree::Make_Non_Terminal_Node(uint partition_key, uint median, /*const*/ node* left, /*const*/ node* right)
{
    return new node(partition_key, median, left, right);
}

node* kdTree::Make_Terminal_Node(valarray_uint& index_list)
{
    return new node(index_list);
}

const node* kdTree::Root() const
{
    return tree;
}

uint kdTree::Dimension() const
{
    return data.Inputs();
}

const IOVector& kdTree :: operator[](uint index) const
{
    return data[index];
}

std::ostream& operator<<(std::ostream& os, const kdTree& kd)
{
    os << *(kd.tree) << std::endl;
    return os;
}
