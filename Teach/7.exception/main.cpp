// Demonstrate exceptions 
// standard library exception hierarchy: https://stdcxx.apache.org/doc/stdlibug/18-2.html

#include <iostream>
#include <stdexcept>

void throwingFunction()
{
	// std::exception doesn't provide custom messages, but other derived exception types do
	// (You can throw anything! e.g. int, class, etc.)
	throw std::exception();
}

int main(int argc, char **argv)
{
	int errorCode = 0;

	try
	{
		throwingFunction();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		errorCode = 1; // non-zero indicates a problem
	}

	return errorCode;
}

