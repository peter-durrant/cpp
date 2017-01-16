// Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using hdd::utility::MS;

int main()
{
	const std::string hostname = MS() << "Hostname: " << hdd::asio::HostName::Get();

	std::cout << hostname << std::endl;

	return 0;
}
