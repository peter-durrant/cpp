// Demonstrate use of own header files
#include "functions.h"

// The included header and the using statements are not in functions.h so need to be defined here.
#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
	int value = 100;
	cout << "original value: " << value << endl;

	// Call the pass-by-value function
	passByValue(value);
	cout << "after pass by value: " << value << endl;

	// Call the pass-by-reference function
	passByReference(value);
	cout << "after pass by reference: " << value << endl;

	return 0;
}

