#pragma once

#include <vector>

class RawData
{
public:
	RawData();
	RawData(uint32_t in, uint32_t out);
	RawData(const std::string& fn);

	uint32_t Inputs() const;
	uint32_t Outputs() const;
	uint32_t Vectors() const;
	uint32_t Series() const;

	const std::vector<double>& operator[](uint32_t index) const;

private:
    std::string filename;
    std::vector<std::vector<double>> data;
    uint32_t inputs;
    uint32_t outputs;

    void Determine_File_Format(std::ifstream& ifs);
};
