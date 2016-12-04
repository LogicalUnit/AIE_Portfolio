#pragma once
/**
	This class holds the routines to procedurally calculate a mesh that represents a simple ceiling fan.
	It is completely scaleable, and can easily be modified with the SetSize() function.
	It accepts a pointer to an empty mesh, and constructs the vertices to make a single fan blade,
	and then duplicates the blade around the Y and X axes to form a 2D fan.
*/


class ProceduralFan
{
public:
	ProceduralFan(void);
	~ProceduralFan(void);

	CTVMesh *BuildFan(CTVMesh *mesh);

	void SetSize(float size = 1.0) { m_Size = size; }; //Scale the size of the fan

private:

	float m_Size;
};
