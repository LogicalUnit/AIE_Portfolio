#ifndef ET_PLANE_H
#define ET_PLANE_H

#include "Common.h"
#include "Vertex.h"

/* Pre-Declaration of Primitives */
class Sphere;
class Triangle;

/* Plane Classification Enum */
enum PlaneClassification {
	PO_Coplanar,
	PO_Intersecting,
	PO_Infront,
	PO_Behind
};

/* Plane Class */
class Plane
{
public:

	/* Plane Coordinates */
	Vertex normal;
	float distance;

	/* Default Constructor */
	Plane(): normal(0.0f, 1.0f, 0.0f), distance(0.0f)
	{
	}

	/* Standard Constructor  */
	Plane(const Vertex &normal_, float distance_ = 0.0f):
		normal(normal_), distance(distance_)
	{
	}

	/* Copy Constructor  */
	Plane(const Plane &plane):
		normal(plane.normal), distance(plane.distance)
	{
	}

	/* Default Destructor */
	~Plane()
	{
	}

	void Setup(float width = 500.f);

	/* Render Plane */
	/*inline void Render(float width = 500.0f) const
	{
		Vertex tangent(Vertex::cross(normal, normal.x > normal.y? Vertex(0.0f, 1.0f, 0.0f) : Vertex(1.0f, 0.0f, 0.0f))),
			   binormal(Vertex::normalized(Vertex::cross(tangent, normal)));

		tangent = Vertex::normalized(Vertex::cross(normal, binormal));
		Vertex origin(normal * -distance);

		glColor4f(0.3f, 0.3f, 0.8f, 0.2f);

		glBegin(GL_QUADS);
			glVertex3fv((GLfloat*)&(origin + (binormal * width) + (tangent * width)));
			glVertex3fv((GLfloat*)&(origin + (binormal * width) + (tangent * -width)));
			glVertex3fv((GLfloat*)&(origin + (binormal * -width) + (tangent * -width)));
			glVertex3fv((GLfloat*)&(origin + (binormal * -width) + (tangent * width)));
		glEnd();
	}*/

	/* Get Classification of a Vertex */
	PlaneClassification Classify(const Vertex &vertex) const;

	/* Get Classification of a Sphere */
	PlaneClassification Classify(const Sphere &sphere, float *distance_ = 0) const;

	/* Get Classification of a Triangle */
	PlaneClassification Classify(const Triangle &triangle) const;

private:

protected:
};

#endif