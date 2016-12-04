#ifndef ET_SPHERE_H
#define ET_SPHERE_H

#include "Common.h"
#include "Vertex.h"

class Triangle;

/* Sphere Class */
class Sphere
{
public:

	/* Sphere Values */
	Vertex origin;
	float radius, radius_squared;

	/* GLU Quadric */
	GLUquadric *quadric;

	/* Default Constructor */
	Sphere(): origin(), radius(1.0f), radius_squared(1.0f)
	{
		quadric = gluNewQuadric();
		gluQuadricNormals(quadric, GLU_SMOOTH);
		mesh = EngineManager::Instance()->GetScene()->CreateMeshBuilder();
		mesh->CreateSphere(30);
	}

	/* Standard Constructor */
	Sphere(const Vertex &origin_, float radius_ = 1.0f):
		origin(origin_), radius(radius_), radius_squared(radius * radius)
	{
		quadric = gluNewQuadric();
		gluQuadricNormals(quadric, GLU_SMOOTH);
		mesh = EngineManager::Instance()->GetScene()->CreateMeshBuilder();
		mesh->CreateSphere(radius);
	}

	/* Default Destructor */
	~Sphere()
	{
		gluDeleteQuadric(quadric);
	}

	/* Render Sphere */
	inline void Render(unsigned char stacks = 32, unsigned char slices = 32) const
	{
		/*glPushMatrix();
			glTranslatef(origin.x, origin.y, origin.z);
			gluSphere(quadric, radius, slices, stacks);
		glPopMatrix();*/
	}

	void Draw();

	/* Test Intersection with Sphere */
	bool Intersect(const Sphere &sphere, float *distance_ = 0) const;

	/* Test Intersection with Line Segment */
	bool Intersect(const Vertex &a, const Vertex &b) const;

	/* Test Intersection with Triangle */
	bool Intersect(const Triangle &triangle, IntersectionData *results = 0) const;

private:
	CTVMesh *mesh;

protected:
};

#endif