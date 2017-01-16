// The main function - entry point into an executable.
//
// argc: number of arguments
// argv: array of arguments (array of character array, each character array terminated with NULL)
// 
// char * is a character array (i.e. a string)
// char ** is an array of character arrays - could also be written as char *argv[]

#include <stdio.h> // printf
#include <assert.h> // assert

int main(int argc, char *argv[])
{
	unsigned int i = 182981;
	unsigned int *ptr = &i; // "&i" gives address (pointer) - store in a pointer "unsigned int *"

	// i = the value
	// ptr = the address of i
	// *ptr = dereference the pointer ptr to get the value i
	printf("%d %d %d\n", i, ptr, *ptr);
	assert(i == *ptr);

	int array[100]; // array of 100 items - allocated memory
	int *arrayPtr = array; // the array object is just a pointer
	int *arrayPtrAlt = &array[0]; // can use the address of the first element of the array to get the pointer
	assert(arrayPtr == arrayPtrAlt);

	for (int i = 0; i < argc; ++i)
	{
		printf("%s\n", argv[i]);
	}

	// Return 0 to indicate success (implicit - can be removed)
	return 0;
}

