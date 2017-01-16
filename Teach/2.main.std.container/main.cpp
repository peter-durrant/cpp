// The main function - entry point into an executable.
//
// argc: number of arguments
// argv: array of arguments (array of character array, each character array terminated with NULL)
//
// Using the C++ standard library - functionality in the std::namespace
// http://en.wikipedia.org/wiki/C%2B%2B_Standard_Library 
//
// Demonstrate a STL container

#include <iostream>
#include <string>
#include <vector>

// Could use "using namespace std" - but would bring whole std namespace into global scope - can be dangerous, so bring in only used types.
// Can use cout, endl, string, vector without std:: namespace since they've been brought into the global scope.
using std::cout;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char **argv)
{
	// Vector of strings
	vector<string> names;

	// Populate vector of names
	names.push_back("Apple");
	names.push_back("Bob");
	names.push_back("Cheese");

	// Curly brackets optional for one-line loops/control statements
	for (int i = 0; i < names.size(); ++i)
		cout << names[i] << endl;

	return 0;
}

