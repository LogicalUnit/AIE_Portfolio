#ifndef ET_TRIANGLE_H
#define ET_TRIANGLE_H

#include "Common.h"
#include "Vertex.h"
#include "Plane.h"

class Sphere;

/* Triangle Class */
class Triangle
{
public:

	/* Plane Coordinates */
	Vertex vertices[3];

	/* Precomputed values */
	Vertex normal;
	Plane plane;

	/* Default Constructor (not to be used) */
	Triangle();

	/* Standard Constructor */
	Triangle(const Vertex &a, const Vertex &b, const Vertex &c);

	/* Default Destructor */
	~Triangle();

	/* Render Triangle */
	 inline void Render()const 
	{
		glBegin(GL_TRIANGLES);
			glVertex3fv((GLfloat*)&vertices[0]);
			glVertex3fv((GLfloat*)&vertices[1]);
			glVertex3fv((GLfloat*)&vertices[2]);
		glEnd();
	}

	/* Get the center point of the triangle */
	Vertex GetCenter() const;

	/* Get the triangles normal */
	const Vertex& GetNormal() const;

	/* Get the plane in which the triangle lies on */
	const Plane& GetPlane() const;

	/* Test intersection with a point */
	bool Intersect(const Vertex &vertex) const;

	/* Test intersection with a sphere */
	bool Intersect(const Sphere &sphere, IntersectionData *results) const;

private:

protected:
};

#endif