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
	static const int numVertices = 8;
	CUSTOMVERTEX vertices[numVertices] =
	{
		{ -Size, Size, -Size, 0xffffff00 },		//TL	= 0
		{ Size, Size, -Size, 0xff00ff00, },		//TR	= 1
		{ -Size, -Size, -Size, 0xffff0000, },	//BL	= 2
		{ Size, -Size, -Size, 0xff0000ff, },	//BR	= 3
		{ -Size, Size, Size, 0xffffffff },		//TL-B	= 4
		{ Size, Size, Size, 0xffffff00 },		//TR-B	= 5
		{ -Size, -Size, Size, 0xffff00ff },		//BL-B	= 6
		{ Size, -Size, Size, 0xffff0000 },		//BR-B	= 7
	};

	//index list
	static const int iNumFaces = 12;
	DWORD indices[iNumFaces*3] =
	{
		2, 1, 3,
		2, 0, 1,
		0, 5, 1,
		0, 4, 5,
		3, 5, 7,
		3, 1, 5,
		2, 6, 0,
		6, 4, 0,
		2, 7, 6,
		2, 3, 7,
		7, 5, 6,
		6, 5, 4
	};
	

	// result - for catching error messages
	HRESULT rv;
	//Number of faces( triangles ) we are going to render
	
	// create the vertex buffer memory
	rv = pDevice->CreateVertexBuffer(
		sizeof( CUSTOMVERTEX ) * numVertices, // the number of bytes in the buffer
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
	sizeof( CUSTOMVERTEX ) * numVertices, // number of bytes to lock
	(VOID**)&pLockedBuffer, // the pointer to hold the locked memory
	0 ); // flags, to write leave this as 0

	// make sure it worked
	if( FAILED( rv ) )
		return;

	// copy - numfaces * 3 = num vertices, then * size of structure
	memcpy( pLockedBuffer, vertices, sizeof( CUSTOMVERTEX ) * numVertices );

	// unlock
	vertexBuffer->Unlock();

	rv = pDevice ->CreateIndexBuffer(
		sizeof( DWORD ) * iNumFaces * 3, // number of indicies in buffer 0, // the usage (used for optimisation)
		0,
		D3DFMT_INDEX32, // index format - unsigned int32
		D3DPOOL_DEFAULT, // what kind of memory to store the indecies on
		&indexBuffer, NULL ); // where to store the index buffer pointer return;

	if ( FAILED(rv) )
		return;

	rv = indexBuffer->Lock(
		0, // offset to lock from
		sizeof( DWORD ) * iNumFaces * 3, // number of bytes to lock
		(VOID**)&pLockedBuffer, // the pointer to hold the locked memory
		0 ); // flags, to write leave this as 0

	if ( FAILED(rv) )
		return;

	// copy - numfaces * 3 = num vertices, then * size of unsigned int32
	memcpy( pLockedBuffer, indices, sizeof( DWORD ) * iNumFaces * 3 );
	indexBuffer->Unlock();
}
