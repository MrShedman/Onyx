#pragma once

#include "Vector.hpp"

#include <iostream>

//#define CULLING 

template <class T>
class Ray
{
public:

	Ray<T>()
		:
		origin(Vector3<T>()),
		direction(Vector3<T>())
	{}

	Ray<T>(const Vector3<T>& origin, const Vector3<T>& direction)
		:
		origin(origin),
		direction(direction.Normalized())
	{}

	bool triangleIntersects(const Vector3<T>& vert0, const Vector3<T>& vert1, const Vector3<T>& vert2, float &t)
	{
		Vector3<T> edge1, edge2, tvec, pvec, qvec;
		float det;
		float u, v;
		const float EPSILON = 0.000001f;

		edge1 = vert1 - vert0;
		edge2 = vert2 - vert0;

		pvec = direction.Cross(edge2);
		det = edge1.Dot(pvec);

#ifdef CULLING
		if (det < EPSILON)
			return false;
		tvec = origin - vert0;

		u = tvec.Dot(pvec);
		if ((u < 0.0f) || (u > det))
			return false;

		qvec = tvec.Cross(edge1);
		v = direction.Dot(qvec);
		if (v < 0.0f || u + v > det)
			return false;

		t = edge2.Dot(qvec) / det;
		return true;
#else
		if (det > -EPSILON && det < EPSILON)
			return false;

		float inv_det = 1.0f / det;
		tvec = origin - vert0;
		u = tvec.Dot(pvec) * inv_det;
		if (u < 0.0f || u > 1.0f)
			return false;

		qvec = tvec.Cross(edge1);

		v = direction.Dot(qvec) * inv_det;
		if (v < 0.0f || u + v > 1.0f)
			return false;

		t = edge2.Dot(qvec) * inv_det;
		return true;
#endif
	}

	Vector3<T> origin;
	Vector3<T> direction;
};

typedef Ray<float> Rayf;