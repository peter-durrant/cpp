#include "rawdata.h"

#include "makestring.h"
#include <fstream>
#include <iostream>

using hdd::utility::MS;

namespace hdd::gamma
{
    RawData::RawData(const std::string& filename)
    {
        std::ifstream inputStream(filename);
        if (!inputStream)
        {
            throw std::runtime_error(MS() << "Unable to open file: " << filename);
        }

        ReadFirstLineAndDetectFileFormat(inputStream);

        uint32_t numberRowsRead = data_.size();
        uint32_t numberCount = 0;
        const uint32_t expectedCountNumbersFoundOnLine = inputs_ + outputs_;

        std::vector<double> numbersFoundOnLine(expectedCountNumbersFoundOnLine);

        while (!inputStream.eof())
        {
            double number;
            inputStream >> number;

            if (!inputStream.eof())
            {
                if (inputStream.fail())
                {
                    throw std::runtime_error(MS() << "Bad character(s) found on line " << numberRowsRead + 1);
                }

                ++numberCount;
                if (numberCount > expectedCountNumbersFoundOnLine)
                {
                    throw std::runtime_error(MS() << "Invalid data format found on line " << numberRowsRead + 1);
                }
                numbersFoundOnLine[numberCount - 1] = number;
                char c;
                inputStream.get(c);
                while (c == ',' || c == ' ' || c == '\r' || c == '\n')
                {
                    if (c == '\r' || c == '\n')
                    {
                        if (expectedCountNumbersFoundOnLine != numberCount && numberCount != 0)
                        {
                            throw std::runtime_error(MS() << "Invalid data format found on line " << numberRowsRead + 1);
                        }

                        if (numberCount != 0)
                        {
                            ++numberRowsRead;
                            data_.push_back(numbersFoundOnLine);
                        }
                        numberCount = 0;
                    }
                    if (inputStream)
                    {
                        inputStream.get(c);
                    }
                    else
                    {
                        c = '\0';
                    }
                }
                if (inputStream)
                {
                    inputStream.putback(c);
                }
            }
        }
        inputStream.close();

        /*	ofstream ofs("out.txt");
            for (uint32_t i = 0; i < num_vectors; i++) {
                for (uint32_t j = 0; j < num_expected; j++) {
                    ofs << data[i][j] << " ";
                }
                ofs << endl;
            }
            ofs.close(); */
        std::cout << "Values expected on each line: " << expectedCountNumbersFoundOnLine << std::endl;
        std::cout << "Inputs: " << inputs_ << ", outputs: " << outputs_ << std::endl;
        std::cout << "Number of vectors: " << numberRowsRead << std::endl;
    }

    void RawData::ReadFirstLineAndDetectFileFormat(std::ifstream& inputStream)
    {
        uint32_t numberCount = 0;
        uint32_t countNumbersFoundOnLine = 0;
        std::vector<double> numbersFoundOnLine;

        //	use the commas to determine the file format
        std::vector<uint32_t> commaPosition;

        // find the first line in the file that starts with at least 1 number
        // commas are expected to separate input and output values
        //        in1 in2 in3, out1 out2
        // a second comma is ignored
        while (!inputStream.eof() && countNumbersFoundOnLine == 0)
        {
            // try to read a number
            double number;
            inputStream >> number;

            if (!inputStream.eof())
            {
                if (inputStream.fail())
                {
                    throw std::runtime_error(MS() << "Bad character(s) found on line " << 1);
                }

                // store the read number
                numbersFoundOnLine.push_back(number);
                ++numberCount;
                char c;
                inputStream.get(c);
                while (c == ',' || c == ' ' || c == '\r' || c == '\n')
                {
                    // at the end of line and some numbers read then store the numbers found on the line
                    if (c == '\r' || c == '\n' && countNumbersFoundOnLine == 0 && numberCount != 0)
                    {
                        countNumbersFoundOnLine = numberCount;
                    }

                    if (c == ',')
                    {
                        commaPosition.push_back(numberCount);
                    }

                    if (inputStream)
                    {
                        inputStream.get(c);
                    }
                    else
                    {
                        c = '\0';
                    }
                }
                if (inputStream)
                {
                    inputStream.putback(c);
                }
            }
        }

        //	determine file structure from commas
        if (commaPosition.size() == 1 || commaPosition.size() == 2)
        {
            inputs_ = commaPosition[0];
            outputs_ = numberCount - inputs_;
        }
        else
        {
            inputs_ = countNumbersFoundOnLine;
            outputs_ = 0;
        }

        //	construct data
        data_.push_back(numbersFoundOnLine);
    }

    uint32_t RawData::Inputs() const
    {
        return inputs_;
    }

    uint32_t RawData::Outputs() const
    {
        return outputs_;
    }

    uint32_t RawData::Vectors() const
    {
        return data_.size();
    }

    uint32_t RawData::Series() const
    {
        return (inputs_ + outputs_);
    }

    const std::vector<double>& RawData::operator[](uint32_t index) const
    {
        return data_[index];
    }
}
