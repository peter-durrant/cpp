#pragma once

#include "transform.h"
#include "iovector.h"
#include "mask.h"

class Data
{
private:
    const RawData& raw_data;
    Transform transform;
    std::vector<IOVector> data;
    Mask input_mask;

    void CreateData();
    void Masked(const IOVector& source, IOVector& dest);

public:
    Data(const RawData& rd);
    Data(const RawData& rd, const std::vector<FormatType>& ft);
    Data(const Data& d, uint sv, uint ev);
    Data(const Data& d, const Mask& m);

    uint Size() const;
    uint Inputs() const;
    uint Outputs() const;

    const IOVector& operator[](uint index) const;
    friend std::ostream& operator<<(std::ostream& os, const Data& d);
};
