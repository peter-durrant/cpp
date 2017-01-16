// Library
#include "functions.h"

#include <iostream>

using std::cout;
using std::endl;

void passByValue(int value)
{
	// Double the value
	value *= 2;
	cout << "in passByValue, value = " << value << endl;
}

void passByReference(int &value)
{
	// Add 50 to the value
	value += 50;
	cout << "in passByReference, value = " << value << endl;
}

