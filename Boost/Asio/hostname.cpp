#include "stdafx.h"
#include "hostname.h"
#include <boost/asio/ip/host_name.hpp>

namespace hdd::asio
{
	std::string HostName::Get()
	{
		boost::system::error_code ec;
		const std::string hostname = boost::asio::ip::host_name(ec);
		return hostname;
	}
}
