#pragma once

#include "transform.h"
#include "iovector.h"
#include "mask.h"

class Data
{
public:
    Data(const RawData& rd);
    Data(const RawData& rd, const std::vector<FormatType>& ft);
    Data(const Data& d, uint32_t sv, uint32_t ev);
    Data(const Data& d, const Mask& m);

    uint32_t Size() const;
    uint32_t Inputs() const;
    uint32_t Outputs() const;

    const IOVector& operator[](uint32_t index) const;
    friend std::ostream& operator<<(std::ostream& os, const Data& d);

private:
    const RawData& raw_data;
    Transform transform;
    std::vector<IOVector> data;
    Mask input_mask;

    void CreateData();
    void Masked(const IOVector& source, IOVector& dest);
};
