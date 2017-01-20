#include "nearest.h"

near_points::near_points() :
    distance(fp_MAX), index_list(0)
{}

near_points::near_points(fp d, uint i) :
    distance(d), index_list(1)
{
    index_list[0] = i;
}

near_points::near_points(fp d, const vect_uint& il) :
    distance(d), index_list(il)
{}

std::ostream& operator<<(std::ostream& os, const near_points& n)
{
    os << "{" << n.distance << "} ";
    os << "{";
    for (uint i = 0; i < n.index_list.size(); i++)
    {
        os << n.index_list[i];
        if (i < n.index_list.size() - 1)
        {
            os << ",";
        }
    }
    os << "}";

    return os;
}

fp Distance(const IOVector& X1, const IOVector& X2, uint power)
{
    const valarray_fp& x1 = X1.Input_Vector();
    const valarray_fp& x2 = X2.Input_Vector();

    fp distance = fp_ZERO;
    for (uint i = 0; i < x1.size(); ++i)
    {
        distance += Raise(x1[i] - x2[i], power);
    }

    return distance;
}

fp Distance(const valarray_fp& x1, const valarray_fp& x2, uint power)
{
    fp distance = fp_ZERO;
    for (uint i = 0; i < x1.size(); ++i)
    {
        distance += Raise(x1[i] - x2[i], power);
    }

    return distance;
}

fp Distance(fp x1, fp x2, uint power)
{
    return Raise(x1 - x2, power);
}

fp Dissim(fp x)
{
    return sqrt(x);
}

Nearest::Nearest(uint q, const kdTree& k, uint p) :
    query_point(k[q]), kdtree(k), pmax(p), near_neighbours(pmax), zero_nn_list(0),
    furthest_distance(fp_MAX), upper_bound(k.Dimension(), fp_MAX), lower_bound(k.Dimension(), -fp_MAX)
{
    Pre_Search();
}

Nearest::Nearest(const IOVector& q, const kdTree& k, uint p) :
    query_point(q), kdtree(k), pmax(p), near_neighbours(pmax), zero_nn_list(0), furthest_distance(fp_MAX),
    upper_bound(k.Dimension(), fp_MAX), lower_bound(k.Dimension(), -fp_MAX)
{
    Pre_Search();
}

void Nearest::Pre_Search()
{
    for (uint i = 0; i < pmax; ++i)
    {
        near_neighbours[i] = new near_points;
    }
    Search(kdtree.Root());
}

Nearest::~Nearest()
{
    for (uint i = 0; i < near_neighbours.size(); ++i)
    {
        if (near_neighbours[i])
        {
            delete near_neighbours[i];
        }
        near_neighbours[i] = 0;
    }
}

void Nearest::Add_Point(uint index)
{
    const IOVector& bucket_point = kdtree[index];
    const fp distance = Distance(query_point, bucket_point); // squared furthest distance
//	const fp distance = Dissim(Distance(query_point, bucket_point));
    if (distance == fp_ZERO) // zero nearest neighbour
    {
        zero_nn_list.push_back(index);
        return;
    }

    if (distance > furthest_distance) return;

    uint i = 0;
    uint j = near_neighbours.size();
    while (i < j && distance > near_neighbours[i]->distance)
    {
        ++i;
    }
    //	if (i == j) return; // should never happen
    if (distance == near_neighbours[i]->distance) // equal nearest neighbour
    {
        near_neighbours[i]->index_list.push_back(index);
    }
    else
    { // insert point into list
        --j;
        delete near_neighbours[j];
        while (j > i)
        {
            near_neighbours[j] = near_neighbours[j - 1];
            --j;
        }
        near_neighbours[i] = new near_points(distance, index);
        furthest_distance = near_neighbours[near_neighbours.size() - 1]->distance;
    }
}

void Nearest::Search(const node* root)
{
    if (root->Terminal())
    {
        for (uint i = 0; i < root->index_list.size(); ++i)
        {
            Add_Point(root->index_list[i]);
        }
        return;
    }

    const uint d = root->partition_key; // partition key
    const fp p = kdtree[root->median].Input_Vector()[d];	// partition value

    const valarray_fp& query_input = query_point.Input_Vector();
    if (query_input[d] < p) // recurse on nearest child
    {
        fp temp = upper_bound[d];
        upper_bound[d] = p;
        Search(root->left);
        upper_bound[d] = temp;

        temp = lower_bound[d];
        lower_bound[d] = p;
        if (Bounds_Overlap_Ball())
        {
            Search(root->right);
        }
        lower_bound[d] = temp;
    }
    else
    {
        fp temp = lower_bound[d];
        lower_bound[d] = p;
        Search(root->right);
        lower_bound[d] = temp;

        temp = upper_bound[d];
        upper_bound[d] = p;
        if (Bounds_Overlap_Ball())
        {
            Search(root->left);
        }
        upper_bound[d] = temp;
    }
    /*
        if (query_input[d] < p) { // recurse on furthest child
            fp temp = lower_bound[d];
            lower_bound[d] = p;
            if (Bounds_Overlap_Ball()) Search(root->right);
          lower_bound[d] = temp;
       } else {
            fp temp = upper_bound[d];
            upper_bound[d] = p;
            if (Bounds_Overlap_Ball()) Search(root->left);
            upper_bound[d] = temp;
       } */
}

/*bool Nearest :: Ball_Within_Bounds()
{
   for (uint d = 0; d < kdtree.Dimension(); d++) {
      if (Distance(query_point.Input_Vector()[d], lower_bound[d]) <= furthest_distance*furthest_distance ||
          Distance(query_point.Input_Vector()[d], upper_bound[d]) <= furthest_distance*furthest_distance)
          return false;
   }
   return true;
}*/

bool Nearest::Bounds_Overlap_Ball()
{
    fp sum = fp_ZERO;
    const valarray_fp& query_input = query_point.Input_Vector();
    for (uint d = 0; d < kdtree.Dimension(); d++)
    {
        if (query_input[d] < lower_bound[d]) // lower than low boundary
        {
            sum += Distance(query_input[d], lower_bound[d]);
            if (sum > furthest_distance) // squared distances
            {
                return false;
            }
            //         if (Dissim(sum) > furthest_distance) return false;
        }
        else if (query_input[d] > upper_bound[d]) // higher than high boundary
        {
            sum += Distance(query_input[d], upper_bound[d]);
            if (sum > furthest_distance) // squared distances
            {
                return false;
            }
            //         if (Dissim(sum) > furthest_distance) return false;
        }
    }
    return true;
}

const std::vector<near_points*>& Nearest::neighbours() const
{
    return near_neighbours;
}

const near_points& Nearest::neighbours(uint p) const
{
    return *near_neighbours[p];
}

std::ostream& operator<<(std::ostream& os, const Nearest& n)
{
    if (n.zero_nn_list.size() > 0)
    {
        os << "Zero nearest neighbours:" << "{";
        for (uint i = 0; i < n.zero_nn_list.size(); ++i)
        {
            os << n.zero_nn_list[i];
            if (i < n.zero_nn_list.size() - 1)
            {
                os << ",";
            }
        }
        os << "}" << std::endl << std::endl;
    }

    for (uint i = 0; i < n.near_neighbours.size(); ++i)
    {
        os << "Near neighbours " << (i + 1) << ":";
        os << (*n.near_neighbours[i]) << std::endl;
    }

    return os;
}
