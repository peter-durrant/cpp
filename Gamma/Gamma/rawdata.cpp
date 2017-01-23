#include "rawdata.h"

#include "makestring.h"
#include <fstream>
#include <iostream>

using hdd::utility::MS;

namespace hdd::gamma
{
    RawData::RawData() :
        data(0), inputs(0), outputs(0)
    {}

    RawData::RawData(uint32_t in, uint32_t out) :
        data(0), inputs(in), outputs(out)
    {}

    RawData::RawData(const std::string& fn) :
        filename(fn)
    {
        std::ifstream ifs(filename.c_str());
        if (!ifs)
        {
            throw std::runtime_error(MS() << "Unable to open file: " << filename);
        }

        Determine_File_Format(ifs);

        uint32_t num_vectors = 1;
        uint32_t num_count = 0;
        uint32_t num_expected = inputs + outputs;
        double number;

        std::vector<double> number_array(num_expected);

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
            for (uint32_t i = 0; i < num_vectors; i++) {
                for (uint32_t j = 0; j < num_expected; j++) {
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
        double number;
        uint32_t num_count = 0;
        uint32_t num_found = 0;
        std::vector<double> number_array;

        //	use the commas to determine the file format
        std::vector<uint32_t> comma_pos;
        uint32_t comma_count = 0;

        while (!ifs.eof() && num_found == 0)
        {
            ifs >> number;

            if (!ifs.eof())
            {
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

    uint32_t RawData::Inputs() const
    {
        return inputs;
    }

    uint32_t RawData::Outputs() const
    {
        return outputs;
    }

    uint32_t RawData::Vectors() const
    {
        return data.size();
    }

    uint32_t RawData::Series() const
    {
        return (inputs + outputs);
    }

    const std::vector<double>& RawData :: operator[](uint32_t index) const
    {
        return data[index];
    }
}
