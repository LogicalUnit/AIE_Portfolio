#include "stdafx.h"
#include "ProceduralFan.h"

ProceduralFan::ProceduralFan(void)
{
	SetSize(30);
}

ProceduralFan::~ProceduralFan(void)
{
}

/**
	Helper fn that is used to calculate the normal of two vectors.
*/
inline cTV_3DVECTOR GetNormal(cTV_3DVECTOR a, cTV_3DVECTOR b, cTV_3DVECTOR c)
{
	CTVMathLibrary *pMath = EngineManager::Instance()->GetMath();
	cTV_3DVECTOR result;

	cTV_3DVECTOR vec1 = b - a;
	cTV_3DVECTOR vec2 = c - a;

	pMath->TVVec3Cross(&result, &vec1, &vec2);
	pMath->TVVec3Normalize(&result, &result);

	return result;
}

/**
	Another helper fn that is used to calculate a vertex from 3 points.
*/
inline cTV_SVERTEX CreateVertex(cTV_3DVECTOR a, cTV_3DVECTOR b, cTV_3DVECTOR c)
{
	cTV_SVERTEX result;
	cTV_3DVECTOR normal = GetNormal(a, b, c);

	result.x = a.x;
	result.y = a.y;
	result.z = a.z;
	result.nx = normal.x;
	result.ny = normal.y;
	result.nz = normal.z;

	return result;
}

/**
	This function creates a procedural mesh. It consists of one blade that is duplicated 7 times for form a 2D ceiling fan shape.
	The geometry is designed so that it can be scaled by any factor -- it can be made smaller or larger easily.
	Pass in a CTVMesh pointer, and this function will create the fan.

	Procedural fan geometry:

	2-------------------------------------3\
										  | \
										  |  \ 4
										  5 - |
										  |	  |
										  |	  |
										  8 - |
										  |  / 6
										  | /
	1-------------------------------------7/
*/

CTVMesh *ProceduralFan::BuildFan(CTVMesh *mesh)
{

	CTVMathLibrary *pMath = EngineManager::Instance()->GetMath();
	static const int numPoints = 8;
	cTV_3DVECTOR point[numPoints];

	point[0] = cTV_3DVECTOR(0.f, 0.f, -m_Size/2.f);

	point[1] = point[0];
	point[1].z += m_Size;

	point[2] = point[1];
	point[2].x += 4*m_Size;

	point[3] = point[2];
	point[3].x += m_Size/2;
	point[3].z -= m_Size/4;

	point[4] = point[3];
	point[4].x -= m_Size/2;

	point[5] = point[4];
	point[5].x += m_Size/2;
	point[5].z -= m_Size/2;

	point[6] = point[5];
	point[6].x -= m_Size/2;
	point[6].z -= m_Size/4;

	point[7] = point[6];
	point[7].z += m_Size/4;

	cTV_SVERTEX vertexArray[10];

	vertexArray[0] = CreateVertex(point[0], point[1], point[6]);
	vertexArray[0].tu = 0;
	vertexArray[0].tv = 1;
	vertexArray[1] = CreateVertex(point[1], point[2], point[0]);
	vertexArray[1].tu = 0;
	vertexArray[1].tv = 0;
	vertexArray[2] = CreateVertex(point[2], point[6], point[1]);
	vertexArray[2].tu = 1;
	vertexArray[2].tv = 0;
	vertexArray[3] = CreateVertex(point[3], point[5], point[4]);
	vertexArray[4] = CreateVertex(point[4], point[3], point[7]);
	vertexArray[5] = CreateVertex(point[5], point[7], point[3]);
	vertexArray[6] = CreateVertex(point[6], point[0], point[2]);
	vertexArray[6].tu = 1;
	vertexArray[6].tv = 1;
	vertexArray[7] = CreateVertex(point[7], point[4], point[5]);
	vertexArray[8] = CreateVertex(point[7], point[5], point[6]);
	vertexArray[9] = CreateVertex(point[4], point[2], point[3]);


	int index[18] = { 0, 1, 2, 
					  6, 0, 2,
					  2, 3, 4,
					  4, 3, 7,
					  5, 7, 3,
					  7, 5, 6  };

	
	mesh->SetGeometry(vertexArray, 10, index, 6);

	
	//Copy and rotate the first fanblade to create the whole fan
	CTVMesh *blades[7];
	CTVMesh *pCurrBlade = mesh;

	CTVTextureFactory *pTex = EngineManager::Instance()->GetTextureFactory();
		
	int tex = pTex->LoadTexture("fanblade_1024_256.jpg", "fanblade");

	mesh->SetTexture(tex);

	blades[0] = mesh->Duplicate();
	blades[0]->RotateX(180);
	blades[0]->MoveRelative(0.f, -0.01f, 0.f);
	blades[0]->AttachTo(cTV_NODETYPE_MESH, mesh->iMeshIndex, 0);

	for (int i = 1; i <= 3; i++)
	{
		blades[i] = pCurrBlade->Duplicate();
		blades[i]->RotateY( i * 90.0f );
		blades[i]->MoveRelative(0.f, 0.01f + 0.01f*i, 0.f);
		blades[i]->AttachTo(cTV_NODETYPE_MESH, mesh->iMeshIndex, i);
		blades[i+1] = blades[i]->Duplicate();
		blades[i+1]->RotateX(180);
		blades[i+1]->MoveRelative(0.f, -0.01f - 0.01f*i, 0.f);
		blades[i+1]->AttachTo(cTV_NODETYPE_MESH, mesh->iMeshIndex, i+1);
		pCurrBlade = blades[i];
	}
	
	return mesh;
}
