#pragma once

#include <boost/noncopyable.hpp>
#include <sstream>
#include <string>

namespace hdd::utility
{
	class MakeString : private boost::noncopyable
	{
	public:
		MakeString() {}
		~MakeString() {}

		template<typename T>
		MakeString& operator<<(const T& value)
		{
			stream_ << value;
			return *this;
		}

		operator std::string() { return stream_.str(); }

	private:
		std::stringstream stream_;
	};

	typedef MakeString MS;
}
