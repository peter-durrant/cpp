#pragma once

#include "rawdata.h"

enum Transformation_Type { Input, Output, TS_Input, TS, Undefined };

struct FormatType
{
    Transformation_Type type;
    uint32_t inputs;
    uint32_t outputs;

    FormatType();
    FormatType(Transformation_Type t, uint32_t in, uint32_t out);
};

class Transform
{
public:
    Transform(const RawData& rd);
    Transform(const RawData& rd, const std::vector<FormatType>& ft);
    uint32_t Inputs() const;
    uint32_t Outputs() const;
    uint32_t Series() const;

    const FormatType& operator[](const uint32_t index) const;

private:
    const RawData& raw_data;
    std::vector<FormatType> format;
    uint32_t inputs;
    uint32_t outputs;

    void Set_Data_Format();
    bool Summarise_All();
    void Summarise(uint32_t variable);
    FormatType Choose();
};
