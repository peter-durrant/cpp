#pragma once

#include <vector>

namespace hdd::gamma
{
    class RawData
    {
    public:
        RawData(const std::string& filename);

        uint32_t Inputs() const;
        uint32_t Outputs() const;
        uint32_t Vectors() const;
        uint32_t Series() const;

        const std::vector<double>& operator[](uint32_t index) const;

    private:
        std::vector<std::vector<double>> data_;
        uint32_t inputs_;
        uint32_t outputs_;

        void ReadFirstLineAndDetectFileFormat(std::ifstream& inputStream);
    };
}