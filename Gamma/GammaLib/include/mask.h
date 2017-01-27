#pragma once

#include <vector>

typedef std::vector<bool> Mask_Type;

namespace hdd::gamma
{
    class Mask
    {
    public:
        Mask(const uint32_t num_in);
        Mask(const Mask& m);

        uint32_t Size() const;
        uint32_t Length() const;
        bool IsEmpty() const;
        void Decrement();
        void Set_Mask(const Mask& m);
        void Set_Mask(const std::string& m);

        const bool operator[](uint32_t index) const;
        friend std::ostream& operator<<(std::ostream& os, const Mask& m);

    private:
        Mask_Type mask_;
    };
}
