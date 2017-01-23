#include "mask.h"

#include <sstream>

namespace hdd::gamma
{
    Mask::Mask(const uint32_t num_in) :
        mask(num_in, true)
    {}

    Mask::Mask(const Mask& m) :
        mask(m.mask)
    {}

    uint32_t Mask::Size()
    {
        return mask.size();
    }

    uint32_t Mask::Length()
    {
        uint32_t length = 0;
        for (uint32_t i = 0; i < mask.size(); ++i)
        {
            if (mask[i])
            {
                length++;
            }
        }
        return length;
    }

    bool Mask::Empty()
    {
        for (uint32_t i = 0; i < mask.size(); ++i)
        {
            if (mask[i]) return false;
        }
        return true;
    }

    void Mask::Decrement()
    {
        for (uint32_t i = 0; i < mask.size(); ++i)
        {
            if (mask[i] == true)
            {
                mask[i] = false;
                for (uint32_t j = i; j > 0; --j)
                {
                    mask[j - 1] = true;
                }
                return;
            }
        }
    }

    void Mask::Set_Mask(const Mask& m)
    {
        mask = m.mask;
    }

    void Mask::Set_Mask(const std::string& m)
    {
        mask.resize(m.size());
        for (uint32_t i = 0; i < m.size(); i++)
        {
            if (m[i] == '0')
            {
                mask[i] = false;
            }
            else
            {
                mask[i] = true;
            }
        }
    }

    const bool Mask :: operator[](uint32_t index) const
    {
        return mask[index];
    }

    /*ostream& operator<<(ostream& os, const Mask& m)
    {
        for (Mask_Type::const_reverse_iterator i = m.mask.rbegin(); i != m.mask.rend(); i++) {
            if (*i == true) os << "1";
          else os << "0";
       }

       return os;
    }*/

    std::ostream& operator<<(std::ostream& os, const Mask& m)
    {
        for (Mask_Type::const_iterator i = m.mask.begin(); i != m.mask.end(); ++i)
        {
            if (*i == true)
            {
                os << "1";
            }
            else
            {
                os << "0";
            }
        }

        return os;
    }
}
