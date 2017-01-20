#pragma once

#include "rawdata.h"

enum Transformation_Type { Input, Output, TS_Input, TS, Undefined };

struct FormatType
{
    Transformation_Type type;
    uint inputs;
    uint outputs;

    FormatType();
    FormatType(Transformation_Type t, uint in, uint out);
};

class Transform
{
private:
    const RawData& raw_data;
    std::vector<FormatType> format;
    uint inputs;
    uint outputs;

    void Set_Data_Format();
    bool Summarise_All();
    void Summarise(uint variable);
    FormatType Choose();

public:
    Transform(const RawData& rd);
    Transform(const RawData& rd, const std::vector<FormatType>& ft);
    uint Inputs() const;
    uint Outputs() const;
    uint Series() const;

    const FormatType& operator[](const uint index) const;
};
