all:
	# create static library
	g++ -c functions.cpp
	ar rvs functions.a functions.o

	# create executable linked to static library
	g++ main.cpp functions.a -o main

