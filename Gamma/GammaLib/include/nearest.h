#pragma once

#include "kdtree.h"

namespace hdd::gamma
{
    struct NearPoints
    {
        double distance_;
        std::vector<uint32_t> indexList_;

        NearPoints();
        NearPoints(double distance, uint32_t i);
        NearPoints(double distance, const std::vector<uint32_t>& indexList);

        friend std::ostream& operator<<(std::ostream& os, const NearPoints& nearPoints);
    };

    double Distance(const IOVector& vector1, const IOVector& vector2, uint32_t power = 2);
    double Distance(const valarray_fp& vector1, const valarray_fp& vector2, uint32_t power = 2);
    double Distance(double vector1, double vector2, uint32_t power = 2);

    class Nearest
    {
    public:
        Nearest(uint32_t queryPointIndex, const KdTree& kdTree, uint32_t pmax);
        Nearest(const IOVector& queryPoint, const KdTree& kdTree, uint32_t pmax);
        virtual ~Nearest();

        const std::vector<NearPoints*>& neighbours() const;
        const NearPoints& neighbours(uint32_t p) const;
        friend std::ostream& operator<<(std::ostream& os, const Nearest& nearest);

    protected:
        const IOVector& queryPoint_;
        const KdTree& kdtree_;
        const uint32_t pmax_;
        std::vector<NearPoints*> nearNeighbours_;
        std::vector<uint32_t> zeroNearestNeighbourList_;
        double furthestDistance_;

        std::vector<double> upperBound_;
        std::vector<double> lowerBound_;

        void AddPoint(uint32_t index);
        void PreSearch();
        void Search(const Node* root);
        bool BoundsOverlapBall();
    };
}
