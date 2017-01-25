#include "data.h"

#include <sstream>

namespace hdd::gamma
{
    Data::Data(const RawData& rd) :
        raw_data(rd), transform(rd), data(0), input_mask(transform.Inputs())
    {
        CreateData();
    }

    Data::Data(const RawData& rd, const std::vector<FormatType>& ft) :
        raw_data(rd), transform(rd, ft), data(0), input_mask(transform.Inputs())
    {
        if (rd.Series() != ft.size())
        {
            throw std::runtime_error("Unable to perform transformation, transformation failed");
        }
        CreateData();
    }

    Data::Data(const Data& d, uint32_t sv, uint32_t ev) :
        raw_data(d.raw_data), transform(d.transform), data(0), input_mask(transform.Inputs())
    {
        // vector indexes [0..M-1]
        if (sv > ev || sv >= d.Size() || ev >= d.Size())
        {
            throw std::runtime_error("Invalid data copy");
        }
        data.resize(ev - sv + 1);
        uint32_t j = 0;
        for (uint32_t i = sv; i <= ev; ++i)
        {
            data[j] = d.data[i];
            ++j;
        }
    }

    Data::Data(const Data& d, const Mask& m) :
        raw_data(d.raw_data), transform(d.transform), data(0), input_mask(m)
    {
        IOVector new_vector(input_mask.Length(), d.Outputs());
        for (uint32_t i = 0; i < d.Size(); ++i)
        {
            Masked(d[i], new_vector);
            data.push_back(new_vector);
        }
    }

    void Data::CreateData()
    {
        uint32_t start_vector = 0;
        uint32_t end_vector = 0;

        for (uint32_t i = 0; i < transform.Series(); ++i)
        {
            if (transform[i].inputs > start_vector)
            {
                start_vector = transform[i].inputs;
            }

            if (transform[i].outputs > end_vector)
            {
                end_vector = transform[i].outputs;
                if (transform[i].type == TS)
                {
                    ++end_vector;
                }
            }
        }

        --start_vector;
        end_vector = raw_data.Vectors() - end_vector;

        if (end_vector <= start_vector)
        {
            throw std::runtime_error("Unable to perform transformation, transformation failed");
        }

        //	const uint32_t num_vectors = end_vector - start_vector + 1;

        const uint32_t __IN = 0;
        const uint32_t __OUT = 1;
        IOVector row(transform.Inputs(), transform.Outputs());

        for (uint32_t i = start_vector; i <= end_vector; ++i)
        {
            uint32_t in_index = 0;
            uint32_t out_index = 0;

            for (uint32_t j = 0; j < raw_data.Series(); ++j)
            {
                for (uint32_t k = 0; k < transform[j].inputs; ++k)
                {
                    row[__IN][in_index++] = raw_data[i + k - transform[j].inputs + 1][j];
                }

                uint32_t offset = 0;
                if (transform[j].type == TS)
                {
                    offset = 1;
                }

                for (uint32_t k = 0; k < transform[j].outputs; ++k)
                {
                    row[__OUT][out_index++] = raw_data[i + k + offset][j];
                }
            }
            data.push_back(row);
        }
    }

    uint32_t Data::Size() const
    {
        return data.size();
    }

    uint32_t Data::Inputs() const
    {
        if (data.size() > 0)
        {
            return data[0].Inputs();
        }
        throw std::runtime_error("Invalid data set");
    }

    uint32_t Data::Outputs() const
    {
        if (data.size() > 0)
        {
            return data[0].Outputs();
        }
        throw std::runtime_error("Invalid data set");
    }

    void Data::Masked(const IOVector& source, IOVector& dest)
    {
        uint32_t j = 0;
        for (uint32_t i = 0; i < input_mask.Size(); ++i)
        {
            if (input_mask[i] == true)
            {
                dest.Input_Vector(j) = source.Input_Vector(i);
                ++j;
            }
        }
        dest.Output_Vector() = source.Output_Vector();
    }

    const std::vector<IOVector>& Data::Get() const
    {
        return data;
    }

    const IOVector& Data::operator[](uint32_t index) const
    {
        return data[index];
    }

    std::ostream& operator<<(std::ostream& os, const Data& d)
    {
        if (!os)
        {
            throw std::runtime_error("Invalid stream");
        }

        //	const uint32_t __IN = 0;
        //	const uint32_t __OUT = 1;

        for (uint32_t i = 0; i < d.data.size(); ++i)
        {
            os << d.data[i];
            os << std::endl;
        }
        return os;
    }
}