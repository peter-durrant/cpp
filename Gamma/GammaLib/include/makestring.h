#pragma once

#include <sstream>
#include <string>

namespace hdd::utility
{
	class MakeString
	{
	public:
        // prevent copy
        MakeString(const MakeString&) = delete;
        MakeString& operator=(const MakeString&) = delete;
        
        MakeString() {}
		virtual ~MakeString() {}

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
