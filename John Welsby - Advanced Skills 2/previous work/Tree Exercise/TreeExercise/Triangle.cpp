#include "stdafx.h"
#include "Triangle.h"
#include "Plane.h"
#include "Sphere.h"

Triangle::Triangle()
{
}

Triangle::Triangle(const Vertex &a, const Vertex &b, const Vertex &c)
{
	vertices[0] = a;
	vertices[1] = b;
	vertices[2] = c;

	normal = Vertex::normalized(Vertex::cross(vertices[0] - vertices[2], vertices[1] - vertices[2]));
	plane = Plane(normal, -Vertex::dot(normal, vertices[0]));
}

Triangle::~Triangle()
{
}

Vertex Triangle::GetCenter() const
{
	return (vertices[0] + vertices[1] + vertices[2]) / 3;
}

const Vertex& Triangle::GetNormal() const
{
	return normal;
}

/* Get the plane in which the triangle lies on */
const Plane& Triangle::GetPlane() const
{
	return plane;
}

bool Triangle::Intersect(const Vertex &vertex) const
{
	Vertex vec0(Vertex::normalized(vertex - vertices[0]));
	Vertex vec1(Vertex::normalized(vertex - vertices[1]));
	Vertex vec2(Vertex::normalized(vertex - vertices[2]));

	float angle(acos(Vertex::dot(vec0, vec1)) +
				acos(Vertex::dot(vec1, vec2)) +
				acos(Vertex::dot(vec2, vec0)));

	return (abs(angle - 2 * 3.141f) < 0.1f);
}

bool Triangle::Intersect(const Sphere &sphere, IntersectionData *results) const
{
	return sphere.Intersect(*this, results);
}