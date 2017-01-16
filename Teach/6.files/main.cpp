// Demonstrate file use

#include <iostream>
#include <fstream>
#include <vector>

void writeFile(const std::string& filename)
{
	// .c_str() - convert std::string to a C-string (pointer to start and NULL terminator)
	std::ofstream outputFileStream(filename.c_str());
	outputFileStream <<
		"AARDVARK\n" <<
		"ABRIDGE\n" <<
		"ABSENT\n" <<
		"ACCOUNT\n" <<
		"ACUMEN";
	// outputFileStream goes out of scope - file stream closed.
}

void readFile(const std::string& filename, std::vector<std::string>& lines)
{
	lines.clear();
	char buffer[1024]; // Read text into this C-string buffer
	std::ifstream inputFileStream(filename.c_str());
	while (inputFileStream)
	{
		inputFileStream.getline(buffer, 1024);
		lines.push_back(buffer);
	}
}

int main(int argc, char **argv)
{
	std::string filename = "storage.txt";
	writeFile(filename);
	
	std::vector<std::string> theLines;
	readFile(filename, theLines);

	// Equivalent output loops
	// Index into vector
	for (int i = 0; i < theLines.size(); ++i)
		std::cout << theLines[i] << std::endl;

	// Iterator
	for (std::vector<std::string>::const_iterator iter = theLines.begin(); iter != theLines.end(); ++iter)
		std::cout << *iter << std::endl;

	// Use auto for shorthand to iterator type
	for (auto iter = theLines.begin(); iter != theLines.end(); ++iter)
		std::cout << *iter << std::endl;

	// C++11 for loop - use reference to avoid copying string from vector into loop variable line
	for (std::string &line: theLines)
		std::cout << line << std::endl;

	return 0;
}

