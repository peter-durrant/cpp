#include "iovector.h"

#include "types.h"
#include <sstream>

namespace hdd::gamma
{
    IOVector::IOVector() :
        input(0), output(0)
    {}

    IOVector::IOVector(uint32_t in, uint32_t out) :
        input(in), output(out)
    {}

    uint32_t IOVector::Inputs() const
    {
        return input.size();
    }

    uint32_t IOVector::Outputs() const
    {
        return output.size();
    }

    void IOVector::Resize(uint32_t in, uint32_t out)
    {
        input.resize(in);
        output.resize(out);
    }

    valarray_fp& IOVector :: operator[](const uint32_t index)
    {
        if (index > 1)
        {
            throw std::runtime_error("Invalid vector operation");
        }

        if (index == 0)
        {
            return input;
        }

        return output;
    }

    const valarray_fp& IOVector::Input_Vector() const
    {
        return input;
    }

    double IOVector::Input_Vector(uint32_t index) const
    {
        if (index > input.size())
        {
            throw std::runtime_error("Invalid vector operation");
        }
        return input[index];
    }

    double& IOVector::Input_Vector(uint32_t index)
    {
        if (index > input.size())
        {
            throw std::runtime_error("Invalid vector operation");
        }
        return input[index];
    }

    const valarray_fp& IOVector::Output_Vector() const
    {
        return output;
    }

    valarray_fp& IOVector::Output_Vector()
    {
        return output;
    }

    double IOVector::Output_Vector(uint32_t index) const
    {
        if (index > output.size())
        {
            throw std::runtime_error("Invalid vector operation");
        }
        return output[index];
    }

    std::ostream& operator<<(std::ostream& os, const IOVector& iov)
    {
        for (uint32_t i = 0; i < iov.input.size(); ++i)
        {
            os << iov.input[i] << " ";
        }

        os << " , ";

        for (uint32_t i = 0; i < iov.output.size(); ++i)
        {
            os << iov.output[i] << " ";
        }

        return os;
    }
}
