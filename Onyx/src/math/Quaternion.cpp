#include "Quaternion.hpp"

#include <iostream>
#include <iomanip>

std::ostream& operator << (std::ostream& os, const Quaternion& q)
{
	os << std::fixed << std::setprecision(5);
	os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
	os << std::endl;

	return os;
}