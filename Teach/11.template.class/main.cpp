// Demonstrate template class

#include <iostream>
#include <string>

// template class has template type(s) listed in angle brackets <>
// Best to implement template classes in header files if providing as part of a library.
template<typename T>
class Person 
{
public:
	Person(const T& field, const std::string &name) :
		field_(field),
		name_(name)
	{}

	void print()
	{
		std::cout << "person: " << name_ << " has " << field_ << std::endl;
	}

protected:
	std::string name_;
	T field_; 
};

int main(int argc, char **argv)
{
	// Template parameter provided during the type declaration.
	Person<std::string> person1("a car", "Zak");
	person1.print();

	Person<int> person2(100, "Bill");
	person2.print();

	return 0;
}

