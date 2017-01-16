// Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	std::cout << hdd::asio::HostName::Get() << std::endl;
	return 0;
}
