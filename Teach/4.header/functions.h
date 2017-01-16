// Guard against multiple inclusion
// (Could also use header guards)
#pragma once

#include <iostream>

using std::cout;
using std::endl;

// Pass by value:
//  (1) to use parameter locally without affecting caller
//  (2) can use const on parameter to indicate that it won't be changed locally, i.e. void passByValue(const int value)
void passByValue(int value)
{
	// Double the value
	value *= 2;
	cout << "in passByValue, value = " << value << endl;
}

// Pass by reference:
//  (1) to update the parameter passed in - can use const on parameter to indicate that it won't be changed locally - and/or
//  (2) to pass objects without a copy
void passByReference(int &value)
{
	// Add 50 to the value
	value += 50;
	cout << "in passByReference, value = " << value << endl;
}

