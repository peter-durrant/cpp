#pragma once

#include "../Base/universal.h"

typedef std::vector<bool> Mask_Type;

class Mask
{
private:
    Mask_Type mask;

public:
    Mask(const uint num_in);
    Mask(const Mask& m);

    uint Size(void);
    uint Length(void);
    bool Empty(void);
    void Decrement(void);
    void Set_Mask(const Mask& m);
    void Set_Mask(const std::string& m);

    const bool operator[](uint index) const;
    friend std::ostream& operator<<(std::ostream& os, const Mask& m);
};
