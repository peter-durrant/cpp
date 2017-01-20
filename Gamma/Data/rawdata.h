#pragma once

#include "../Base/universal.h"

class RawData
{
private:
    std::string filename;
    std::vector<std::vector<fp> > data;
	uint inputs;
	uint outputs;

	void Determine_File_Format(std::ifstream& ifs);

public:
	RawData();
	RawData(uint in, uint out);
	RawData(const std::string& fn);

	uint Inputs() const;
	uint Outputs() const;
	uint Vectors() const;
	uint Series() const;

	const std::vector<fp>& operator[](uint index) const;
};
