// Demonstrate use of own header files
#include "functions.h"

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

