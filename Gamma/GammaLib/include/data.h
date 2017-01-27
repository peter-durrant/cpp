#pragma once

#include "transform.h"
#include "iovector.h"
#include "mask.h"

namespace hdd::gamma
{
    class Data
    {
    public:
        Data(const RawData& rawData);
        Data(const RawData& rawData, const std::vector<FormatType>& columnType);
        Data(const Data& data, uint32_t startIndex, uint32_t endIndex);
        Data(const Data& data, const Mask& inputMask);

        uint32_t Size() const;
        uint32_t Inputs() const;
        uint32_t Outputs() const;

        const IOVector& operator[](uint32_t index) const;
        friend std::ostream& operator<<(std::ostream& os, const Data& data);

    private:
        const RawData& rawData_;
        Transform transform_;
        std::vector<IOVector> data_;
        Mask inputMask_;

        void CreateData();
        void Masked(const IOVector& source, IOVector& dest);
    };
}
