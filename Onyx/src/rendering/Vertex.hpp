#pragma once

#include "math\Vector.hpp"

struct Vertex
{
	Vertex() : position(), normal() {}

	Vertex(Vector3f position) : position(position) {}

	Vertex(Vector3f position, Vector3f normal) : position(position), normal(normal) {}

	Vector3f position;

	Vector3f normal;
};