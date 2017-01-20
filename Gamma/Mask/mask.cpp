#include "mask.h"

Mask::Mask(const uint num_in) :
    mask(num_in, true)
{}

Mask::Mask(const Mask& m) :
    mask(m.mask)
{}

uint Mask::Size(void)
{
    return mask.size();
}

uint Mask::Length(void)
{
    uint length = 0;
    for (uint i = 0; i < mask.size(); ++i)
    {
        if (mask[i])
        {
            length++;
        }
    }
    return length;
}

bool Mask::Empty(void)
{
    for (uint i = 0; i < mask.size(); ++i)
    {
        if (mask[i]) return false;
    }
    return true;
}

void Mask::Decrement(void)
{
    for (uint i = 0; i < mask.size(); ++i)
    {
        if (mask[i] == true)
        {
            mask[i] = false;
            for (uint j = i; j > 0; j--)
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
    for (uint i = 0; i < m.size(); i++)
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

const bool Mask :: operator[](uint index) const
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
