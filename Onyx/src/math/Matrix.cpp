#include "Matrix.hpp"

#include <iostream>
#include <iomanip>

std::ostream& operator << (std::ostream& os, const Matrix4f& m)
{
	for (int i = 0; i < 4; ++i)
	{
		os << std::fixed << std::setprecision(5);
		os << "[";
		for (int j = 0; j < 3; ++j)
		{
			os << std::left << std::setfill(' ') << std::setw(10) << m[i][j];
		}
		os << m[i][3] << "]" << std::endl;
	}

	return os;
}