#pragma once

#include <valarray>
#include <vector>

template<class ValueType>
class Range
{
public:
    Range() :
        min_(std::numeric_limits<ValueType>::max()),
        max_(std::numeric_limits<ValueType>::min())
    {

    }

    ~Range()
    {

    }

    void Update(const ValueType& value)
    {
        if (value < min_)
        {
            min_ = value;
        }
        
        if (value > max_)
        {
            max_ = value;
        }
    }

    ValueType Spread()
    {
        if (min_ > max_)
        {
            throw std::runtime_error("Failed to calculate range");
        }

        return max_ - min_;
    }

private:
    ValueType min_;
    ValueType max_;
};

class InputOutputDataPoint
{
public:
    InputOutputDataPoint(int inputs, int outputs) :
        inputs_(inputs, 0),
        outputs_(outputs, 0)
    {
    }

    InputOutputDataPoint(const std::vector<double>& inputs, const std::vector<double>& outputs) :
        inputs_(inputs),
        outputs_(outputs)
    {
    }

    const std::vector<double>& Inputs() const
    {
        return inputs_;
    }

    const std::vector<double>& Outputs() const
    {
        return outputs_;
    }

private:
    std::vector<double> inputs_;
    std::vector<double> outputs_;
};

template<class DataType>
class KdTree
{
public:
    KdTree(const std::valarray<DataType> data, const std::size_t bucketSize = 4) :
        data_(data),
        bucketSize_(bucketSize)
    {
        if (data.size() <= bucketSize)
        {
            // finished
        }
        else
        {
            // partition on dimension with greatest spread
            auto maxSpreadDimension = MaxSpread(data);
        }
    }

    ~KdTree()
    {

    }

    int MaxSpread(const std::valarray<DataType>& data)
    {
        size_t numberOfInputs = data[0].Inputs().size();
        std::valarray<Range<double>> spread(numberOfInputs);
        for (const DataType& dataPoint : data)
        {
            for(size_t i = 0; i < numberOfInputs; ++i)
            {
                spread[i].Update(dataPoint.Inputs()[i]);
            }
        }

        int maxSpreadDimension = 0;
        double maxSpread = spread[0].Spread();
        for(size_t i = 1; i < numberOfInputs; ++i)
        {
            if (spread[i].Spread() > maxSpread)
            {
                maxSpread = spread[i].Spread();
                maxSpreadDimension = i;
            }
        }
        return maxSpreadDimension;
    }

private:
    KdTree();
    const int bucketSize_;
    const std::valarray<DataType>& data_;
};
