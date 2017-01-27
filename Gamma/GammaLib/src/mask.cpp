#include "mask.h"

#include <sstream>

namespace hdd::gamma
{
    Mask::Mask(const uint32_t num_in) :
        mask_(num_in, true)
    {}

    Mask::Mask(const Mask& m) :
        mask_(m.mask_)
    {}

    uint32_t Mask::Size() const
    {
        return mask_.size();
    }

    uint32_t Mask::Length() const
    {
        uint32_t length = 0;
        for (uint32_t i = 0; i < mask_.size(); ++i)
        {
            if (mask_[i])
            {
                ++length;
            }
        }
        return length;
    }

    bool Mask::IsEmpty() const
    {
        for (uint32_t i = 0; i < mask_.size(); ++i)
        {
            if (mask_[i])
            {
                return false;
            }
        }
        return true;
    }

    void Mask::Decrement()
    {
        for (uint32_t i = 0; i < mask_.size(); ++i)
        {
            if (mask_[i] == true)
            {
                mask_[i] = false;
                for (uint32_t j = i; j > 0; --j)
                {
                    mask_[j - 1] = true;
                }
                return;
            }
        }
    }

    void Mask::Set_Mask(const Mask& m)
    {
        mask_ = m.mask_;
    }

    void Mask::Set_Mask(const std::string& m)
    {
        mask_.resize(m.size());
        for (uint32_t i = 0; i < m.size(); i++)
        {
            if (m[i] == '0')
            {
                mask_[i] = false;
            }
            else
            {
                mask_[i] = true;
            }
        }
    }

    const bool Mask::operator[](uint32_t index) const
    {
        return mask_[index];
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
        for (Mask_Type::const_iterator i = m.mask_.begin(); i != m.mask_.end(); ++i)
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
