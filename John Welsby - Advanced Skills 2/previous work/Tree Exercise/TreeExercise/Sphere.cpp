#include "stdafx.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"

bool Sphere::Intersect(const Sphere &sphere, float *distance_) const
{
	Vertex dist(origin - sphere.origin);
	float distance(Vertex::dot(dist, dist));

	if(distance_) *distance_ = distance;

	return distance < radius + sphere.radius;
}

bool Sphere::Intersect(const Vertex &a, const Vertex &b) const
{
	Vertex direction(Vertex::normalized(a - b));
	float t(Vertex::dot(origin - a, direction));

	Vertex closest;
	if(t < 0.0f) closest = a;
	else if(t > 1.0f) closest = b;
	else closest = (direction * t);

	return (Vertex::mag2(origin - closest) < radius_squared);
}

/* Assumes the sphere intersects the triangles plane. */
bool Sphere::Intersect(const Triangle &triangle, IntersectionData *results) const
{
	float distance(Vertex::dot(origin, triangle.plane.normal) + triangle.plane.distance);

	/* If we intersect the plane, make sure we intersect the reigon that the triangle is on */
	if(abs(distance) < radius)
	{
		/* Project sphere's origin onto the triangle's plane */
		Vertex temp(origin + (triangle.plane.normal * abs(distance)));

		if(	/* Sphere's projected origin is inside triangle */
			triangle.Intersect(temp) ||

			/* Sphere intersects edge of triangle */
			Intersect(triangle.vertices[0], triangle.vertices[1]) ||
			Intersect(triangle.vertices[0], triangle.vertices[2]) ||
			Intersect(triangle.vertices[1], triangle.vertices[2]))
		{
			/* Set Intersection Data */
			if(results)
			{
				results->distance = distance;
				results->normal = triangle.plane.normal;
			}

			/* Sphere Intersects the Triangle */
			return true;
		}
	}

	/* No Intersection */
	return false;
}

void Sphere::Draw()
{
	mesh->SetPosition(origin.x, origin.y, origin.z);
	
}