#include "stdafx.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"

PlaneClassification Plane::Classify(const Vertex &vertex) const
{
	float point_distance(Vertex::dot(vertex, normal) + distance);

	return (point_distance > 0.0f? PO_Infront : (point_distance < 0.0f? PO_Behind : PO_Coplanar));
}

PlaneClassification Plane::Classify(const Sphere &sphere, float *distance_) const
{
	float point_distance(Vertex::dot(sphere.origin, normal) + distance);
	if(distance_) *distance_ = point_distance - sphere.radius;

	return (point_distance > sphere.radius? PO_Infront : (point_distance < -sphere.radius? PO_Behind : PO_Intersecting));
}

PlaneClassification Plane::Classify(const Triangle &triangle) const
{
	bool front(false), back(false);

	for(unsigned char i(0); i < 3; i++)
	{
		switch(Classify(triangle.vertices[i]))
		{
			case PO_Infront:
				front = true;
				break;

			case PO_Behind:
				back = true;
				break;
		}
	}

	if(front)
	{
		if(back) return PO_Intersecting;
		else return PO_Infront;
	}
	else if(back) return PO_Behind;

	return PO_Coplanar;
}

void Plane::Setup(float width)
{
	Vertex tangent(Vertex::cross(normal, normal.x > normal.y? Vertex(0.0f, 1.0f, 0.0f) : Vertex(1.0f, 0.0f, 0.0f))),
			   binormal(Vertex::normalized(Vertex::cross(tangent, normal)));

		tangent = Vertex::normalized(Vertex::cross(normal, binormal));
		Vertex origin(normal * -distance);

		Vertex v1, v2, v3, v4;

		v1 = origin + (binormal * width) + (tangent * width);
		v2 = origin + (binormal * width) + (tangent * -width);
		v3 = origin + (binormal * -width) + (tangent * -width);
		v4 = origin + (binormal * -width) + (tangent * width);

		int index[6] = {	0, 1, 2, 
							2, 3, 0		};

		cTV_SVERTEX vertex[4];
		
		vertex[0] = cTV_SVERTEX(v1.x, v1.y, v1.z, 0.0, 0.0, 0.0, 0.0, 0.0);
		vertex[1] = cTV_SVERTEX(v2.x, v2.y, v2.z, 0.0, 0.0, 0.0, 0.0, 0.0);
		vertex[2] = cTV_SVERTEX(v3.x, v3.y, v3.z, 0.0, 0.0, 0.0, 0.0, 0.0);
		vertex[3] = cTV_SVERTEX(v4.x, v4.y, v4.z, 0.0, 0.0, 0.0, 0.0, 0.0);

		CTVMesh *mesh = EngineManager::Instance()->GetScene()->CreateMeshBuilder();
		mesh->SetGeometry(vertex, 4, index, 2);
		mesh->SetColor( 0x8800ffaa, true);
		mesh->SetCullMode( cTV_DOUBLESIDED );
		mesh->SetLightingMode(cTV_LIGHTING_MANAGED);
		mesh->SetAlphaTest(false, 128, false); 
		mesh->SetBlendingMode( cTV_BLEND_ADDALPHA );
		
}