#include "iovector.h"

#include "types.h"
#include <sstream>

namespace hdd::gamma
{
    IOVector::IOVector(uint32_t in, uint32_t out) :
        input_(in), output_(out)
    {}

    uint32_t IOVector::Inputs() const
    {
        return input_.size();
    }

    uint32_t IOVector::Outputs() const
    {
        return output_.size();
    }

    void IOVector::Resize(uint32_t in, uint32_t out)
    {
        input_.resize(in);
        output_.resize(out);
    }

    valarray_fp& IOVector::operator[](const uint32_t index)
    {
        if (index > 1)
        {
            throw std::runtime_error("Invalid vector operation");
        }

        if (index == 0)
        {
            return input_;
        }

        return output_;
    }

    const valarray_fp& IOVector::InputVector() const
    {
        return input_;
    }

    double IOVector::InputValue(uint32_t index) const
    {
        if (index > input_.size())
        {
            throw std::runtime_error("Invalid vector operation");
        }
        return input_[index];
    }

    double& IOVector::InputValue(uint32_t index)
    {
        if (index > input_.size())
        {
            throw std::runtime_error("Invalid vector operation");
        }
        return input_[index];
    }

    const valarray_fp& IOVector::OutputVector() const
    {
        return output_;
    }

    valarray_fp& IOVector::OutputVector()
    {
        return output_;
    }

    double IOVector::OutputValue(uint32_t index) const
    {
        if (index > output_.size())
        {
            throw std::runtime_error("Invalid vector operation");
        }
        return output_[index];
    }

    std::ostream& operator<<(std::ostream& os, const IOVector& iov)
    {
        for (uint32_t i = 0; i < iov.input_.size(); ++i)
        {
            os << iov.input_[i] << " ";
        }

        os << " , ";

        for (uint32_t i = 0; i < iov.output_.size(); ++i)
        {
            os << iov.output_[i] << " ";
        }

        return os;
    }
}
