// Demonstrate class destructors

#include <iostream>
#include <string>

class Person 
{
public:
	// Constructor with initialisation list (after :)
	Person(const int age, const std::string &name) :
		age_(age),
		name_(name)
	{}

	// Destructor - if virtual will be called correctly (if this derived classes are ever destroyed by a pointer to a base class) 
	virtual ~Person()
	{
		// Deallocate any resources
		std::cout << "~Person(): Person object (" << name_ << "," << age_ << ") destroyed" << std::endl;
	}

	void printPerson()
	{
		std::cout << "someone: " << name_ << " is " << age_ << " years old" << std::endl;
	}

protected:
	int age_;
	std::string name_;
};

class Worker : public Person
{
public:
	Worker(const int age, const std::string &name) :
		Person(age, name)
	{}

	~Worker()
	{
		// Deallocate any resources
		std::cout << "~Worker(): Worker object (" << name_ << "," << age_ << ") destroyed" << std::endl;
	}
};

int main(int argc, char **argv)
{
	// Stack allocated memory
	Person personA(1, "Apple");
	personA.printPerson();

	Person personB(2, "Bob");
	personB.printPerson();

	// introduce scope - personC will be destroyed when scope closes
	{
		Person personC(3, "Cheese");
		personC.printPerson();
	}

	Worker worker1(20, "Dee");
	worker1.printPerson();

	// Heap allocated memory (access the object using the -> operator)
	// Allocate using new
	// De-allocate using delete - missing delete will cause a memory leak 
	Person *worker2 = new Worker(50, "Evan");
	worker2->printPerson();
	delete worker2;

	return 0;
}

