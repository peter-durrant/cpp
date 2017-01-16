// The main function - entry point into an executable.
//
// argc: number of arguments
// argv: array of arguments (array of character array, each character array terminated with NULL)
//
// Using the C++ standard library - functionality in the std::namespace
// http://en.wikipedia.org/wiki/C%2B%2B_Standard_Library 

#include <iostream>

int main(int argc, char **argv)
{
	// standard library string - easier to use than C-strings (char *)
	std::string name;

	// std::cout - write to the console
	// std::endl - write a new line
	// std::cin - read from the console
	std::cout << "Please enter your name (followed by return): " << std::endl;
	std::cin >> name;
	std::cout << "Hello " << name << std::endl;

	return 0;
}

