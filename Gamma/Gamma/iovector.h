#pragma once

//class IOVectorExcept : public ExceptionBase {};
//
//class BadOperator : public IOVectorExcept
//{
//public:
//	BadOperator();
//};

class IOVector
{
private:
    valarray_fp input;
    valarray_fp output;

public:
    IOVector();
    IOVector(uint in, uint out);
    uint Inputs() const;
    uint Outputs() const;
    valarray_fp& operator[](const uint index);

    void Resize(uint in, uint out);

    const valarray_fp& Input_Vector() const;
    fp Input_Vector(uint index) const;
    fp& Input_Vector(uint index);
    const valarray_fp& Output_Vector() const;
    valarray_fp& Output_Vector();
    fp Output_Vector(uint index) const;

    friend std::ostream& operator<<(std::ostream& os, const IOVector& iov);
};
