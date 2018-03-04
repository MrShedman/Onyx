#pragma once

#include "Vector.hpp"

//#undef max
//#undef min

template <class T>
class AABB
{
public:

	AABB<T>()
		:
		max(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest()),
		min(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max())
	{}

	AABB<T>(const AABB<T>& rhs)
		:
		max(rhs.max),
		min(rhs.min)
	{}

	AABB<T>(const Vector3<T>& min, const Vector3<T>& max)
		:
		max(max),
		min(min)
	{}

	AABB<T>(const std::vector<Vector3<T>>& points)
	{
		include(points);
	}

	void include(const Vector3<T>& point)
	{
		min = min.Min(point);
		max = max.Max(point);
	}

	void include(const std::vector<Vector3<T>>& points)
	{
		for (auto&p : points)
			include(p);
	}

	T getWidth() const
	{
		return max.x - min.x;
	}

	T getHeight() const
	{
		return max.y - min.y;
	}

	T getDepth() const
	{
		return max.z - min.z;
	}

	T getVolume() const
	{
		return getWidth() * getHeight() * getDepth();
	}

public:

	Vector3<T> min;
	Vector3<T> max;
};

typedef AABB<float> AABBf;
typedef AABB<int> AABBi;