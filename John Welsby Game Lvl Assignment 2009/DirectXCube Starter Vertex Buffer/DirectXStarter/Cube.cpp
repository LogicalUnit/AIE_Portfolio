#include "StdAfx.h"
#include "Cube.h"

Cube::Cube(void)
{
}

Cube::~Cube(void)
{
}

void Cube::Init(IDirect3DDevice9* pDevice)
{
	// vertex offset distance
	float Size = 10.f;
	// the list of vertices
	static const int numFaces = 12;
	CUSTOMVERTEX vertices[numFaces*3] =
	{	
	//  triangle1
		{ -Size, -Size, -Size, 0xffff0000, },
		{ Size, Size, -Size, 0xff00ff00, },
		{ Size, -Size, -Size, 0xff0000ff, },
	//	triangle2
		{ -Size, -Size, -Size, 0xffff0000, },
		{ -Size, Size, -Size, 0xffffff00 },
		{ Size, Size, -Size, 0xff00ff00, },	
	//	triangle3
		{ -Size, Size, -Size, 0xffffff00 },	
		{ Size, Size, Size, 0xffffff00 },
		{ Size, Size, -Size, 0xff00ff00, },
	//	triangle4
		{ -Size, Size, -Size, 0xffffff00 },
		{ -Size, Size, Size, 0xffffffff },
		{ Size, Size, Size, 0xffffff00 },
	//	triangle5
		{ Size, -Size, -Size, 0xff0000ff, },
		{ Size, Size, Size, 0xffffff00 },
		{ Size, -Size, Size, 0xffff0000 },
	//	triangle6
		{ Size, -Size, -Size, 0xff0000ff, },
		{ Size, Size, -Size, 0xff00ff00, },
		{ Size, Size, Size, 0xffffff00 },
	//	triangle7
		{ -Size, -Size, -Size, 0xffff0000, },
		{ -Size, -Size, Size, 0xffff00ff },
		{ -Size, Size, -Size, 0xffffff00 },	
	//	triangle8
		{ -Size, -Size, Size, 0xffff00ff },
		{ -Size, Size, Size, 0xffffffff },	
		{ -Size, Size, -Size, 0xffffff00 },
	//	triangle9
		{ -Size, -Size, -Size, 0xffff0000, },
		{ Size, -Size, Size, 0xffff0000 },
		{ -Size, -Size, Size, 0xffff00ff },
	//	triangle10
		{ -Size, -Size, -Size, 0xffff0000, },
		{ Size, -Size, -Size, 0xff0000ff, },
		{ Size, -Size, Size, 0xffff0000 },
	//	triangle11
		{ Size, -Size, Size, 0xffff0000 },
		{ Size, Size, Size, 0xffffff00 },
		{ -Size, -Size, Size, 0xffff00ff },
	//	triangle12
		{ -Size, -Size, Size, 0xffff00ff },
		{ Size, Size, Size, 0xffffff00 },
		{ -Size, Size, Size, 0xffffffff },

	};
	

	// result - for catching error messages
	HRESULT rv;
	//Number of faces( triangles ) we are going to render
	
	// create the vertex buffer memory
	rv = pDevice->CreateVertexBuffer(
		sizeof( CUSTOMVERTEX ) * numFaces * 3, // the number of bytes in the buffer
		0, // the usage (used for optimisation)
		D3DFVF_CUSTOMVERTEX, // FVF flags defined in cube.h
		D3DPOOL_DEFAULT, // what kind of memory to store the vertices on( video ) 
		&vertexBuffer, NULL ); // where to store the vertex buffer pointer
	
	// make sure it worked
	if( FAILED( rv) )
		return;
	
	// now lock the buffer, copy the data to it, and then unlock it
	BYTE* pLockedBuffer;
	
	// lock
	rv = vertexBuffer->Lock(
	0, // offset to lock from
	sizeof( CUSTOMVERTEX ) * numFaces * 3, // number of bytes to lock
	(VOID**)&pLockedBuffer, // the pointer to hold the locked memory
	0 ); // flags, to write leave this as 0

	// make sure it worked
	if( FAILED( rv ) )
		return;

	// copy - numfaces * 3 = num vertices, then * size of structure
	memcpy( pLockedBuffer, vertices, sizeof( CUSTOMVERTEX ) * numFaces * 3 );

	// unlock
	vertexBuffer->Unlock();
}
