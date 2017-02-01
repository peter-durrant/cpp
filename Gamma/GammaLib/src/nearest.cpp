#include "nearest.h"

#include <sstream>

namespace hdd::gamma
{
    NearPoints::NearPoints() :
        distance_(std::numeric_limits<double>::max()), indexList_(0)
    {}

    NearPoints::NearPoints(double distance, uint32_t i) :
        distance_(distance), indexList_(1)
    {
        indexList_[0] = i;
    }

    NearPoints::NearPoints(double distance, const std::vector<uint32_t>& indexList) :
        distance_(distance), indexList_(indexList)
    {}

    std::ostream& operator<<(std::ostream& os, const NearPoints& nearPoints)
    {
        os << "{" << nearPoints.distance_ << "} ";
        os << "{";
        for (uint32_t i = 0; i < nearPoints.indexList_.size(); i++)
        {
            os << nearPoints.indexList_[i];
            if (i < nearPoints.indexList_.size() - 1)
            {
                os << ",";
            }
        }
        os << "}";

        return os;
    }

    double Distance(const IOVector& vector1, const IOVector& vector2, uint32_t power)
    {
        const valarray_fp& inputs1 = vector1.InputVector();
        const valarray_fp& inputs2 = vector2.InputVector();

        double distance = 0;
        for (uint32_t i = 0; i < inputs1.size(); ++i)
        {
            distance += pow(inputs1[i] - inputs2[i], power);
        }

        return distance;
    }

    double Distance(const valarray_fp& vector1, const valarray_fp& vector2, uint32_t power)
    {
        double distance = 0;
        for (uint32_t i = 0; i < vector1.size(); ++i)
        {
            distance += pow(vector1[i] - vector2[i], power);
        }

        return distance;
    }

    double Distance(double vector1, double vector2, uint32_t power)
    {
        return pow(vector1 - vector2, power);
    }

    Nearest::Nearest(uint32_t queryPointIndex, const KdTree& kdTree, uint32_t pmax) :
        queryPoint_(kdTree[queryPointIndex]), kdtree_(kdTree), pmax_(pmax), nearNeighbours_(pmax_), zeroNearestNeighbourList_(0),
        furthestDistance_(std::numeric_limits<double>::max()), upperBound_(kdTree.Dimension(), std::numeric_limits<double>::max()), lowerBound_(kdTree.Dimension(), -std::numeric_limits<double>::max())
    {
        PreSearch();
    }

    Nearest::Nearest(const IOVector& queryPoint, const KdTree& kdTree, uint32_t pmax) :
        queryPoint_(queryPoint), kdtree_(kdTree), pmax_(pmax), nearNeighbours_(pmax_), zeroNearestNeighbourList_(0), furthestDistance_(std::numeric_limits<double>::max()),
        upperBound_(kdTree.Dimension(), std::numeric_limits<double>::max()), lowerBound_(kdTree.Dimension(), -std::numeric_limits<double>::max())
    {
        PreSearch();
    }

    void Nearest::PreSearch()
    {
        for (uint32_t i = 0; i < pmax_; ++i)
        {
            nearNeighbours_[i] = new NearPoints;
        }
        Search(kdtree_.Root());
    }

    Nearest::~Nearest()
    {
        for (uint32_t i = 0; i < nearNeighbours_.size(); ++i)
        {
            if (nearNeighbours_[i])
            {
                delete nearNeighbours_[i];
            }
            nearNeighbours_[i] = 0;
        }
    }

    void Nearest::AddPoint(uint32_t index)
    {
        const IOVector& bucket_point = kdtree_[index];
        const double distance = Distance(queryPoint_, bucket_point); // squared furthest distance
        if (distance == 0) // zero nearest neighbour
        {
            zeroNearestNeighbourList_.push_back(index);
            return;
        }

        if (distance > furthestDistance_)
        {
            return;
        }

        uint32_t i = 0;
        uint32_t j = nearNeighbours_.size();
        while (i < j && distance > nearNeighbours_[i]->distance_)
        {
            ++i;
        }
        if (distance == nearNeighbours_[i]->distance_) // equal nearest neighbour
        {
            nearNeighbours_[i]->indexList_.push_back(index);
        }
        else
        { // insert point into list
            --j;
            delete nearNeighbours_[j];
            while (j > i)
            {
                nearNeighbours_[j] = nearNeighbours_[j - 1];
                --j;
            }
            nearNeighbours_[i] = new NearPoints(distance, index);
            furthestDistance_ = nearNeighbours_[nearNeighbours_.size() - 1]->distance_;
        }
    }

    void Nearest::Search(const Node* root)
    {
        if (root->Terminal())
        {
            for (uint32_t i = 0; i < root->indexList_.size(); ++i)
            {
                AddPoint(root->indexList_[i]);
            }
            return;
        }

        const uint32_t d = root->partitionKey_; // partition key
        const double p = kdtree_[root->median_].InputVector()[d];	// partition value

        const valarray_fp& query_input = queryPoint_.InputVector();
        if (query_input[d] < p) // recurse on nearest child
        {
            double temp = upperBound_[d];
            upperBound_[d] = p;
            Search(root->left_);
            upperBound_[d] = temp;

            temp = lowerBound_[d];
            lowerBound_[d] = p;
            if (BoundsOverlapBall())
            {
                Search(root->right_);
            }
            lowerBound_[d] = temp;
        }
        else
        {
            double temp = lowerBound_[d];
            lowerBound_[d] = p;
            Search(root->right_);
            lowerBound_[d] = temp;

            temp = upperBound_[d];
            upperBound_[d] = p;
            if (BoundsOverlapBall())
            {
                Search(root->left_);
            }
            upperBound_[d] = temp;
        }
    }

    bool Nearest::BoundsOverlapBall()
    {
        double sum = 0;
        const valarray_fp& query_input = queryPoint_.InputVector();
        for (uint32_t d = 0; d < kdtree_.Dimension(); d++)
        {
            if (query_input[d] < lowerBound_[d]) // lower than low boundary
            {
                sum += Distance(query_input[d], lowerBound_[d]);
                if (sum > furthestDistance_) // squared distances
                {
                    return false;
                }
            }
            else if (query_input[d] > upperBound_[d]) // higher than high boundary
            {
                sum += Distance(query_input[d], upperBound_[d]);
                if (sum > furthestDistance_) // squared distances
                {
                    return false;
                }
            }
        }
        return true;
    }

    const std::vector<NearPoints*>& Nearest::neighbours() const
    {
        return nearNeighbours_;
    }

    const NearPoints& Nearest::neighbours(uint32_t p) const
    {
        return *nearNeighbours_[p];
    }

    std::ostream& operator<<(std::ostream& os, const Nearest& nearest)
    {
        if (nearest.zeroNearestNeighbourList_.size() > 0)
        {
            os << "Zero nearest neighbours:" << "{";
            for (uint32_t i = 0; i < nearest.zeroNearestNeighbourList_.size(); ++i)
            {
                os << nearest.zeroNearestNeighbourList_[i];
                if (i < nearest.zeroNearestNeighbourList_.size() - 1)
                {
                    os << ",";
                }
            }
            os << "}" << std::endl << std::endl;
        }

        for (uint32_t i = 0; i < nearest.nearNeighbours_.size(); ++i)
        {
            os << "Near neighbours " << (i + 1) << ":";
            os << (*nearest.nearNeighbours_[i]) << std::endl;
        }

        return os;
    }
}
