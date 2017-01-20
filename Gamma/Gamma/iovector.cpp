#include "universal.h"
#include "iovector.h"

IOVector::IOVector() :
    input(EMPTY), output(EMPTY)
{}

IOVector::IOVector(uint in, uint out) :
    input(in), output(out)
{}

uint IOVector::Inputs() const
{
    return input.size();
}

uint IOVector::Outputs() const
{
    return output.size();
}

void IOVector::Resize(uint in, uint out)
{
    input.resize(in);
    output.resize(out);
}

valarray_fp& IOVector :: operator[](const uint index)
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

fp IOVector::Input_Vector(uint index) const
{
    if (index > input.size())
    {
        throw std::runtime_error("Invalid vector operation");
    }
    return input[index];
}

fp& IOVector::Input_Vector(uint index)
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

fp IOVector::Output_Vector(uint index) const
{
    if (index > output.size())
    {
        throw std::runtime_error("Invalid vector operation");
    }
    return output[index];
}

std::ostream& operator<<(std::ostream& os, const IOVector& iov)
{
    for (uint i = 0; i < iov.input.size(); ++i)
    {
        os << iov.input[i] << " ";
    }

    os << " , ";

    for (uint i = 0; i < iov.output.size(); ++i)
    {
        os << iov.output[i] << " ";
    }

    return os;
}
