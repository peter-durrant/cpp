// Demonstrate class constructors

#include <iostream>
#include <string>

// Class is default-private (public access to members requires "public:")
class Person 
{
public:
	// Constructor with initialisation list (after :)
	Person(const int age, const std::string& name) :
		age_(age),
		name_(name)
	{}

	Person(const Person &person)
	{
		if (this != &person)
		{
			age_ = person.age_;
			name_ = person.name_;
		}
	}

	int getAge() const
	{
		return age_;
	}

	std::string getName() const
	{
		return name_;
	}

private:
	int age_;
	std::string name_;
};

void printPerson(const Person &person)
{
	std::cout << "person: " << person.getName() << " is " << person.getAge() << " years old" << std::endl;
}

int main(int argc, char **argv)
{
	Person person(21, "Smithy");
	printPerson(person);

	return 0;
}

