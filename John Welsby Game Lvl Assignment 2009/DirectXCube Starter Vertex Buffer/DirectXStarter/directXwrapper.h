#pragma once


class DirectXWrapper
{
public:
	DirectXWrapper();
	~DirectXWrapper();

	HRESULT Initialise( HWND hWnd, bool windowed );
	void Render();
	void Cleanup();

private:
	IDirect3DDevice9*		pDevice;
	D3DPRESENT_PARAMETERS	d3dpp;
	LPDIRECT3D9				pD3D;

	D3DDISPLAYMODE			mode;
	D3DCAPS9				pCaps;

	// for camera 
	float		kCameraMovementSpeed,kCameraRotationSpeed;
	double		PreviousFrameTime;
	CFPCamera	*pCamera;
	Input		*pInput;

	// declare and initialize the matrices
	D3DXMATRIX	matView;
	D3DXMATRIX	matProjection;

	Cube		m_Cube;
	
};