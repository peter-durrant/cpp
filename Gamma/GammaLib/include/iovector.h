#pragma once

#include "types.h"

namespace hdd::gamma
{
    class IOVector
    {
    public:
        IOVector(uint32_t in, uint32_t out);
        uint32_t Inputs() const;
        uint32_t Outputs() const;
        valarray_fp& operator[](const uint32_t index);

        void Resize(uint32_t in, uint32_t out);

        const valarray_fp& InputVector() const;
        double InputValue(uint32_t index) const;
        double& InputValue(uint32_t index);
        const valarray_fp& OutputVector() const;
        valarray_fp& OutputVector();
        double OutputValue(uint32_t index) const;

        friend std::ostream& operator<<(std::ostream& os, const IOVector& iov);

    private:
        valarray_fp input_;
        valarray_fp output_;
    };
}
