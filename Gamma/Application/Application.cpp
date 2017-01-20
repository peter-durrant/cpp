// Application.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Gamma/rawdata.h"
#include "../Gamma/data.h"
#include "../Gamma/transform.h"
#include "../Gamma/gamma.h"
#include <iostream>

int main()
{
    RawData rawdata("Z:\\GitHub\\cpp\\Gamma\\Application\\Hen1000.asc");
    FormatType format(Transformation_Type::TS, 3, 1);
    std::vector<FormatType> formats = { format };
    Transform transform(rawdata, formats);
    std::cout << "Inputs: " << transform.Inputs() << std::endl;
    std::cout << "Outputs: " << transform.Outputs() << std::endl;
    Data data(rawdata, formats);
    Gamma gamma(data, 10);
    std::cout << gamma << std::endl;
    return 0;
}

