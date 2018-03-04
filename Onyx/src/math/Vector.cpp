#include "Vector.hpp"
#include "Quaternion.hpp"

#include <iostream>
#include <iomanip>

template<>
Vector3f Vector3f::Rotate(const Quaternion& rotation) const
{
	Vector3f u(rotation.x, rotation.y, rotation.z);

	//// Extract the scalar part of the quaternion
	float s = rotation.w;

	Vector3f t = u.Cross(*this) * 2.0f;
	return *this + t * s + u.Cross(t);
}

std::ostream& operator << (std::ostream& os, const Vector2f& v)
{
	os << std::fixed << std::setprecision(5);

	os << "(" << v.x << ", " << v.y << ")" << std::endl;

	return os;
}

std::ostream& operator << (std::ostream& os, const Vector3f& v)
{
	os << std::fixed << std::setprecision(5);

	os << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;

	return os;
}