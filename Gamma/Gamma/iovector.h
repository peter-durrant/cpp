#pragma once

#include "types.h"

namespace hdd::gamma
{
    class IOVector
    {
    public:
        IOVector();
        IOVector(uint32_t in, uint32_t out);
        uint32_t Inputs() const;
        uint32_t Outputs() const;
        valarray_fp& operator[](const uint32_t index);

        void Resize(uint32_t in, uint32_t out);

        const valarray_fp& Input_Vector() const;
        double Input_Vector(uint32_t index) const;
        double& Input_Vector(uint32_t index);
        const valarray_fp& Output_Vector() const;
        valarray_fp& Output_Vector();
        double Output_Vector(uint32_t index) const;

        friend std::ostream& operator<<(std::ostream& os, const IOVector& iov);

    private:
        valarray_fp input;
        valarray_fp output;
    };
}
