#pragma once

#include <string>

namespace hdd::asio
{
	class HostName
	{
	public:
		static std::string Get();

	private:
		HostName();
	};
}
