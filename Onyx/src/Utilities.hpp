#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace Util
{
	void consoleWait();

	std::vector<std::string> split(const std::string& s, char delim);

	template <class T>
	inline std::string toString(const T value)
	{
		std::stringstream stream;
		stream << value;
		return stream.str();
	}
}