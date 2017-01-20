#include "transform.h"

FormatType::FormatType()
    : type(Undefined), inputs(0), outputs(0)
{}

FormatType::FormatType(Transformation_Type t, uint in, uint out)
    : type(t), inputs(in), outputs(out)
{}

Transform::Transform(const RawData& rd)
    : raw_data(rd), format(0), inputs(0), outputs(0)
{
    if (raw_data.Inputs() == 0)
    {
        throw std::runtime_error("Unable to perform transformation, invalid data format");
    }

    FormatType ft;
    if (raw_data.Outputs() == 0)
    {
        for (uint i = 0; i < raw_data.Inputs(); ++i)
        {
            ft.type = TS;
            ft.inputs = 0;
            ft.outputs = 0;
            format.push_back(ft);
        }
    }
    else
    {
        for (uint i = 0; i < raw_data.Inputs(); ++i)
        {
            ft.type = Input;
            ft.inputs = 1;
            ft.outputs = 0;
            format.push_back(ft);
        }
        for (uint i = 0; i < raw_data.Outputs(); ++i)
        {
            ft.type = Output;
            ft.inputs = 0;
            ft.outputs = 1;
            format.push_back(ft);
        }
    }

    Set_Data_Format();

    for (uint i = 0; i < format.size(); ++i)
    {
        inputs += format[i].inputs;
        outputs += format[i].outputs;
    }

    if (inputs == 0 || outputs == 0)
    {
        throw std::runtime_error("Unable to perform transformation, there must be at least one input and one output");
    }
}

Transform::Transform(const RawData& rd, const std::vector<FormatType>& ft)
    : raw_data(rd), format(ft), inputs(0), outputs(0)
{
    if (raw_data.Inputs() == 0 || ft.size() != rd.Series())
    {
        throw std::runtime_error("Unable to perform transformation, invalid data format");
    }

    for (uint i = 0; i < format.size(); ++i)
    {
        inputs += format[i].inputs;
        outputs += format[i].outputs;
    }

    if (inputs == 0 || outputs == 0)
    {
        throw std::runtime_error("Unable to perform transformation, there must be at least one input and one output");
    }
}

void Transform::Set_Data_Format()
{
    if (!Summarise_All())
    {
        for (uint i = 0; i < raw_data.Inputs() + raw_data.Outputs(); ++i) {
            std::cout << std::endl << std::endl << "-------------------------------" << std::endl;
            Summarise(i);
            format[i] = Choose();
        }
    }
}

bool Transform::Summarise_All()
{
    std::cout << std::endl;
    for (uint i = 0; i < format.size(); ++i)
    {
        Summarise(i);
    }

    if (raw_data.Inputs() > 0 && raw_data.Outputs() > 0)
    {
        std::cout << std::endl << "Do you wish to change these settings? (y/n) [n]" << std::endl;
        char c = '\0';
        while (c != '\n' && c != 'y' && c != 'n') std::cin.get(c);
        if (c == '\n' || c == 'n') return true;
    }
    return false;
}

void Transform::Summarise(uint variable)
{
    std::cout << "Series " << (variable + 1) << ": ";
    switch (format[variable].type)
    {
    case Input:
        std::cout << "Input" << std::endl;
        break;

    case Output:
        std::cout << "Output" << std::endl;
        break;

    case TS_Input:
        std::cout << "Time Series (Input)" << std::endl;
        break;

    case TS:
        std::cout << "Time Series" << std::endl;
        break;
    }
    std::cout << "First value: " << raw_data[0][variable] << std::endl << std::endl;
}

FormatType Transform::Choose()
{
    std::cout << "Select transformation type:" << std::endl;
    std::cout << "  1. Input" << std::endl;
    std::cout << "  2. Output" << std::endl;
    std::cout << "  3. Time Series (Input)" << std::endl;
    std::cout << "  4. Time Series" << std::endl;

    uint choice = 0;
    while (choice < 1 || choice > 4)
    {
        std::cout << std::endl << "Enter choice [1..4]" << std::endl;
        std::cin >> choice;
    }

    FormatType ft;
    switch (choice) 
    {
    case 1:
        ft.type = Input;
        ft.inputs = 1;
        ft.outputs = 0;
        break;

    case 2:
        ft.type = Output;
        ft.inputs = 0;
        ft.outputs = 1;
        break;

    case 3:
        ft.type = TS_Input;
        std::cout << "Enter number of input time steps: " << std::endl;
        std::cin >> ft.inputs;
        ft.outputs = 0;
        break;

    case 4:
        ft.type = TS;
        std::cout << "Enter number of input time steps: " << std::endl;
        std::cin >> ft.inputs;
        std::cout << "Enter number of output time steps: " << std::endl;
        std::cin >> ft.outputs;
        break;
    }

    return ft;
}

uint Transform::Inputs() const
{
    return inputs;
}

uint Transform::Outputs() const
{
    return outputs;
}

uint Transform::Series() const
{
    return raw_data.Series();
}

const FormatType& Transform :: operator[](const uint index) const
{
    if (index > format.size())
    {
        throw std::runtime_error("System error, index out of bounds");
    }
    return format[index];
}
