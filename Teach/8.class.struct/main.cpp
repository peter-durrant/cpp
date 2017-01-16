// Demonstrate equivalence of struct and class 

#include <iostream>
#include <string>

// Convention: mark members with a trailing underscore
// struct is default-public (no explicit "public:" required)
struct MyStruct
{
	int age_;
	std::string name_; 
};

// Class is default-private (public access to members requires "public:")
class MyClass
{
public:
	int age_;
	std::string name_;

	// Add a copy operator to assign a MyStruct to a MyClass
	MyClass& operator=(const MyStruct &rhs)
	{
		age_ = rhs.age_;
		name_ = rhs.name_;
		return *this;
	}	
};

int main(int argc, char **argv)
{
	MyStruct myStruct;
	MyClass myClass;

	myStruct.age_ = 21;
	myStruct.name_ = "Smithy";

	// Compiler automatically generates a copy operator.
	MyStruct myStructCopy;
	myStructCopy = myStruct;

	// Cannot assign different values from structures without an explicit assignment operator.
	myClass = myStruct;

	std::cout << "myStruct: " << myStruct.name_ << " is " << myStruct.age_ << " years old" << std::endl;
	std::cout << "myStructCopy: " << myStructCopy.name_ << " is " << myStructCopy.age_ << " years old" << std::endl;
	std::cout << "myClass: " << myClass.name_ << " is " << myClass.age_ << " years old" << std::endl;

	return 0;
}

