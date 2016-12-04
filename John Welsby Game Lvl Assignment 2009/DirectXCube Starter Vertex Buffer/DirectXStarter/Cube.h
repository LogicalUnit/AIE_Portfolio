#pragma once

struct CUSTOMVERTEX
{
	FLOAT x, y, z; // The 3D position for the vertex
	D3DCOLOR colour; // colour of vertex ( argb values )
};

// define the FVF here to make flags more acessable for changes
// Setting the untransformed vertex position flag and the colour flag
// in the order they are in the custom vertex structure above
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE )

class Cube
{
private:
	IDirect3DVertexBuffer9* vertexBuffer;
	IDirect3DIndexBuffer9* indexBuffer;

public:
	Cube(void);
	~Cube(void);

	void	Init(IDirect3DDevice9* pDevice);

	inline IDirect3DVertexBuffer9 *GetVertexBuffer() { return vertexBuffer; };
	inline IDirect3DIndexBuffer9 *GetIndexBuffer() { return indexBuffer; };
};
