#ifndef ET_VERTEX_H
#define ET_VERTEX_H

#include <math.h>

/* Standard Vertex Class */
class Vertex
{
public:

	/* Vertex Coordinates */
	float x, y, z, w;

	/* Default Constructor */
	Vertex(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, float w_ = 0.0f):
		x(x_), y(y_), z(z_), w(w_)
	{
	}

	/* Default Destructor */
	~Vertex() {}

	/* Logical Equals Operator */
	inline bool operator==(const Vertex &vertex)
	{
		return (x == vertex.x && y == vertex.y &&z == vertex.z && w == vertex.w);
	}

	/* Addition Operator */
	inline void operator+=(const Vertex &vertex)
	{
		x += vertex.x;
		y += vertex.y;
		z += vertex.z;
		w += vertex.w;
	}

	/* Subtraction Operator */
	inline void operator-=(const Vertex &vertex)
	{
		x -= vertex.x;
		y -= vertex.y;
		z -= vertex.z;
		w -= vertex.w;
	}

	/* Multiplication Operator */
	inline void operator*=(const Vertex &vertex)
	{
		x *= vertex.x;
		y *= vertex.y;
		z *= vertex.z;
		w *= vertex.w;
	}

	/* Division Operator */
	inline void operator/=(const Vertex &vertex)
	{
		x /= vertex.x;
		y /= vertex.y;
		z /= vertex.z;
		w /= vertex.w;
	}

	/* Multiplication Operator */
	inline void operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
	}

	/* Division Operator */
	inline void operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
	}

	/* Addition Operator */
	inline Vertex operator+(const Vertex &vertex) const
	{
		return Vertex(x + vertex.x, y + vertex.y, z + vertex.z, w + vertex.w);
	}

	/* Subtraction Operator */
	inline Vertex operator-(const Vertex &vertex) const
	{
		return Vertex(x - vertex.x, y - vertex.y, z - vertex.z, w - vertex.w);
	}

	/* Multiplication Operator */
	inline Vertex operator*(const Vertex &vertex) const
	{
		return Vertex(x * vertex.x, y * vertex.y, z * vertex.z, w * vertex.w);
	}

	/* Division Operator */
	inline Vertex operator/(const Vertex &vertex) const
	{
		return Vertex(x / vertex.x, y / vertex.y, z / vertex.z, w / vertex.w);
	}

	/* Multiplication Operator */
	inline Vertex operator*(float scalar) const
	{
		return Vertex(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	/* Division Operator */
	inline Vertex operator/(float scalar) const
	{
		return Vertex(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	/* Cross Product */
	inline static Vertex cross(const Vertex &a, const Vertex &b)
	{
		return Vertex(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
	}

	/* Dot Product */
	inline static float dot(const Vertex &a, const Vertex &b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
	}

	/* Product */
	inline static float product(const Vertex &vertex)
	{
		return vertex.x * vertex.y * vertex.z;
	}

	/* Magnitude */
	inline static float mag(const Vertex &vertex)
	{
		return sqrt(vertex.x * vertex.x + vertex.y * vertex.y + vertex.z * vertex.z + vertex.w * vertex.w);
	}

	/* Magnitude Squared*/
	inline static float mag2(const Vertex &vertex)
	{
		return (vertex.x * vertex.x + vertex.y * vertex.y + vertex.z * vertex.z + vertex.w * vertex.w);
	}

	/* Normalized vertex */
	inline static Vertex normalized(const Vertex &vertex)
	{
		float temp(1.0f / Vertex::mag(vertex));

		return Vertex(vertex.x * temp, vertex.y * temp, vertex.z * temp);
	}

private:

protected:
};

static Vertex operator*(const float &scalar, const Vertex &vertex)
{
	return Vertex(vertex.x * scalar, vertex.y * scalar, vertex.z * scalar, vertex.w * scalar);
}

static Vertex operator/(const float &scalar, const Vertex &vertex)
{
	return Vertex(vertex.x / scalar, vertex.y / scalar, vertex.z / scalar, vertex.w / scalar);
}

/* Intersection Data structure */
struct IntersectionData
{
	Vertex normal;
	float distance;
};

#endif