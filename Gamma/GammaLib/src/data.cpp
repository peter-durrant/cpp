#include "data.h"

#include <sstream>

namespace hdd::gamma
{
    Data::Data(const RawData& rawData) :
        rawData_(rawData), transform_(rawData), data_(0), inputMask_(transform_.Inputs())
    {
        CreateData();
    }

    Data::Data(const RawData& rawData, const std::vector<FormatType>& columnType) :
        rawData_(rawData), transform_(rawData, columnType), data_(0), inputMask_(transform_.Inputs())
    {
        if (rawData.Series() != columnType.size())
        {
            throw std::runtime_error("Unable to perform transformation, transformation failed");
        }
        CreateData();
    }

    Data::Data(const Data& data, uint32_t startIndex, uint32_t endIndex) :
        rawData_(data.rawData_), transform_(data.transform_), data_(0), inputMask_(transform_.Inputs())
    {
        // vector indexes [0..M-1]
        if (startIndex > endIndex || startIndex >= data.Size() || endIndex >= data.Size())
        {
            throw std::runtime_error("Invalid data copy");
        }
        data_.resize(endIndex - startIndex + 1);
        uint32_t newIndex = 0;
        for (uint32_t originalIndex = startIndex; originalIndex <= endIndex; ++originalIndex)
        {
            data_[newIndex] = data.data_[originalIndex];
            ++newIndex;
        }
    }

    Data::Data(const Data& data, const Mask& inputMask) :
        rawData_(data.rawData_), transform_(data.transform_), data_(0), inputMask_(inputMask)
    {
        IOVector maskedDataVector(inputMask_.Length(), data.Outputs());
        for (uint32_t i = 0; i < data.Size(); ++i)
        {
            Masked(data[i], maskedDataVector);
            data_.push_back(maskedDataVector);
        }
    }

    void Data::CreateData()
    {
        uint32_t startIndex = 0;
        uint32_t endIndex = 0;

        for (uint32_t i = 0; i < transform_.Series(); ++i)
        {
            if (transform_[i].inputs > startIndex)
            {
                startIndex = transform_[i].inputs;
            }

            if (transform_[i].outputs > endIndex)
            {
                endIndex = transform_[i].outputs;
                if (transform_[i].type == TS)
                {
                    ++endIndex;
                }
            }
        }

        --startIndex;
        endIndex = rawData_.Vectors() - endIndex;

        if (endIndex <= startIndex)
        {
            throw std::runtime_error("Unable to perform transformation, transformation failed");
        }

        const uint32_t In = 0;
        const uint32_t Out = 1;
        IOVector row(transform_.Inputs(), transform_.Outputs());

        for (uint32_t index = startIndex; index <= endIndex; ++index)
        {
            uint32_t inputIndex = 0;
            uint32_t outputIndex = 0;

            for (uint32_t j = 0; j < rawData_.Series(); ++j)
            {
                for (uint32_t k = 0; k < transform_[j].inputs; ++k)
                {
                    row[In][inputIndex++] = rawData_[index + k - transform_[j].inputs + 1][j];
                }

                uint32_t offset = 0;
                if (transform_[j].type == TS)
                {
                    offset = 1;
                }

                for (uint32_t k = 0; k < transform_[j].outputs; ++k)
                {
                    row[Out][outputIndex++] = rawData_[index + k + offset][j];
                }
            }
            data_.push_back(row);
        }
    }

    uint32_t Data::Size() const
    {
        return data_.size();
    }

    uint32_t Data::Inputs() const
    {
        if (data_.size() > 0)
        {
            return data_[0].Inputs();
        }
        throw std::runtime_error("Invalid data set");
    }

    uint32_t Data::Outputs() const
    {
        if (data_.size() > 0)
        {
            return data_[0].Outputs();
        }
        throw std::runtime_error("Invalid data set");
    }

    void Data::Masked(const IOVector& source, IOVector& dest)
    {
        uint32_t j = 0;
        for (uint32_t i = 0; i < inputMask_.Size(); ++i)
        {
            if (inputMask_[i] == true)
            {
                dest.Input_Vector(j) = source.Input_Vector(i);
                ++j;
            }
        }
        dest.Output_Vector() = source.Output_Vector();
    }

    const IOVector& Data::operator[](uint32_t index) const
    {
        return data_[index];
    }

    std::ostream& operator<<(std::ostream& os, const Data& data)
    {
        if (!os)
        {
            throw std::runtime_error("Invalid stream");
        }

        for (uint32_t i = 0; i < data.data_.size(); ++i)
        {
            os << data.data_[i];
            os << std::endl;
        }
        return os;
    }
}