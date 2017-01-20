#include "rawdata.h"
#include <fstream>
#include "../Base/makestring.h"

using hdd::utility::MS;

RawData::RawData()
    : data(0), inputs(0), outputs(0)
{}

RawData::RawData(uint in, uint out)
    : data(0), inputs(in), outputs(out)
{}

RawData::RawData(const std::string& fn)
    : filename(fn)
{
    std::ifstream ifs(filename.c_str());
    if (!ifs)
    {
        throw std::runtime_error(MS() << "Unable to open file: " << filename);
    }

    Determine_File_Format(ifs);

    uint num_vectors = 1;
    uint num_count = 0;
    uint num_expected = inputs + outputs;
    fp number;

    std::vector<fp> number_array(num_expected);

    while (!ifs.eof())
    {
        ifs >> number;

        if (!ifs.eof())
        {
            if (ifs.fail())
            {
                throw std::runtime_error(MS() << "Bad character(s) found on line " << num_vectors + 1 << " in file: " << filename);
            }
            else
            {
                ++num_count;
                if (num_count > num_expected)
                {
                    throw std::runtime_error(MS() << "Invalid data format found on line " << num_vectors + 1 << " in file: " << filename);
                }
                number_array[num_count - 1] = number;
                char c;
                ifs.get(c);
                while (c == ',' || c == ' ' || c == '\r' || c == '\n')
                {
                    if (c == '\r' || c == '\n')
                    {
                        if (num_expected != num_count && num_count != 0)
                        {
                            throw std::runtime_error(MS() << "Invalid data format found on line " << num_vectors + 1 << " in file: " << filename);
                        }
                        if (num_count != 0)
                        {
                            ++num_vectors;
                            data.push_back(number_array);
                        }
                        num_count = 0;
                    }
                    if (ifs)
                    {
                        ifs.get(c);
                    }
                    else
                    {
                        c = '\0';
                    }
                }
                if (ifs)
                {
                    ifs.putback(c);
                }
            }
        }
    }
    ifs.close();

    /*	ofstream ofs("out.txt");
        for (uint i = 0; i < num_vectors; i++) {
            for (uint j = 0; j < num_expected; j++) {
                ofs << data[i][j] << " ";
            }
            ofs << endl;
        }
        ofs.close(); */
    std::cout << "Values expected on each line: " << num_expected << std::endl;
    std::cout << "Inputs: " << inputs << ", outputs: " << outputs << std::endl;
    std::cout << "Number of vectors: " << num_vectors << std::endl;
}

void RawData::Determine_File_Format(std::ifstream& ifs)
{
    fp number;
    uint num_count = 0;
    uint num_found = 0;
    std::vector<fp> number_array;

    //	use the commas to determine the file format
    std::vector<uint> comma_pos;
    uint comma_count = 0;

    while (!ifs.eof() && num_found == 0)
    {
        ifs >> number;

        if (!ifs.eof()) {
            if (ifs.fail())
            {
                throw std::runtime_error(MS() << "Bad character(s) found on line " << 1 << " in file: " << filename);
            }
            else
            {
                number_array.push_back(number);
                ++num_count;
                char c;
                ifs.get(c);
                while (c == ',' || c == ' ' || c == '\r' || c == '\n')
                {
                    if (c == '\r' || c == '\n' && num_found == 0 && num_count != 0)
                    {
                        num_found = num_count;
                    }

                    if (c == ',')
                    {
                        ++comma_count;
                        comma_pos.push_back(num_count);
                    }

                    if (ifs)
                    {
                        ifs.get(c);
                    }
                    else
                    {
                        c = '\0';
                    }
                }
                if (ifs)
                {
                    ifs.putback(c);
                }
            }
        }
    }

    //	determine file structure from commas
    if (comma_count == 1 || comma_count == 2)
    {
        inputs = comma_pos[0];
        outputs = num_count - inputs;
    }
    else
    {
        inputs = num_found;
        outputs = 0;
    }

    //	construct data
    data.push_back(number_array);
}

uint RawData::Inputs() const
{
    return inputs;
}

uint RawData::Outputs() const
{
    return outputs;
}

uint RawData::Vectors() const
{
    return data.size();
}

uint RawData::Series() const
{
    return (inputs + outputs);
}

const std::vector<fp>& RawData :: operator[](uint index) const
{
    return data[index];
}

