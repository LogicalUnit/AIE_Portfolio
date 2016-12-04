#include "stdafx.h"


DirectXWrapper::DirectXWrapper()
{
	pDevice = NULL;

	// create the camera
	pCamera = new CFPCamera(D3DXVECTOR3(0, 0, -40));

	kCameraMovementSpeed = 0.4f;
	kCameraRotationSpeed = 0.01f;

	// create new input
	pInput = new Input();

	// set the timer
	PreviousFrameTime = pctimer();

	
}

DirectXWrapper::~DirectXWrapper()
{
	if( pInput )
	{
		delete ( pInput );
		pInput = NULL;
	}

	if( pCamera )
	{
		delete ( pCamera );
		pCamera = NULL;
	}
}

HRESULT DirectXWrapper::Initialise(HWND hWnd, bool windowed)
{

	// create the Direct3D device object
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;

	
	// initialise the structure used to create the device
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed   = windowed;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if( !windowed )
	{
		
		d3dpp.BackBufferWidth   = 1024;
		d3dpp.BackBufferHeight  = 768;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		
		// double check the mode availablility
		if( FAILED( pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT , &mode)))
			return E_FAIL;

		d3dpp.BackBufferFormat = mode.Format;
		d3dpp.Flags = mode.Format;
	}
	if( windowed )
	{
		// double check the mode availablility
		if( FAILED( pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT , &mode)))
			return E_FAIL;

		d3dpp.BackBufferFormat = mode.Format;

		if(FAILED(pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
			      d3dpp.BackBufferFormat, d3dpp.BackBufferFormat, FALSE)))
			return E_FAIL;
	}


	// create the Direct3D device
	// parameters are: default adapter, a hardware abstraction layer device, software vertex programming
	if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp, &pDevice ) ) )
	{
		return E_FAIL;
	}

	//Turn lighting off - as we are not using it in this example
	pDevice->SetRenderState(D3DRS_LIGHTING, false);


	// Set up matrix
	RECT rct;
	GetClientRect(hWnd,&rct);
	D3DXMATRIX matProj;

	float aspect = (rct.right-rct.left) / (float)(rct.bottom - rct.top);
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, aspect, 1.0f, 100.0f );

	pDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	m_Cube.Init(pDevice);

	return S_OK;
}


void DirectXWrapper::Render()
{
	//Calculate Previous Frame time
	double delta = (pctimer() - PreviousFrameTime) / 1.f;

	/********************************************/
	
	//Update the input
	pInput->Update();

	//Move left
	if ( pInput->IsKeyDown( DIK_A ) )
		pCamera->MoveRight(-kCameraMovementSpeed);
	//Move Right
	if ( pInput->IsKeyDown( DIK_D ) )
		pCamera->MoveRight(kCameraMovementSpeed);
	//Move forward
	if ( pInput->IsKeyDown( DIK_W ) )
		pCamera->MoveForward(kCameraMovementSpeed);
	//Move back
	if ( pInput->IsKeyDown( DIK_S ) )
		pCamera->MoveForward(-kCameraMovementSpeed);

	//Move up
	if ( pInput->IsKeyDown( DIK_Q ) )
		pCamera->MoveUp(kCameraMovementSpeed);
	//Move down
	if ( pInput->IsKeyDown( DIK_Z ) )
		pCamera->MoveUp(-kCameraMovementSpeed);

	// rotations
	//left
	if ( pInput->IsKeyDown( DIK_NUMPAD4 ) )
		pCamera->Yaw(-kCameraRotationSpeed);
	//right
	if ( pInput->IsKeyDown( DIK_NUMPAD6 ) )
		pCamera->Yaw(kCameraRotationSpeed);

	//up
	if ( pInput->IsKeyDown( DIK_NUMPAD8 ) )
		pCamera->Pitch(-kCameraRotationSpeed);
	//down
	if ( pInput->IsKeyDown( DIK_NUMPAD2 ) )
		pCamera->Pitch(kCameraRotationSpeed);

	//roll left
	if ( pInput->IsKeyDown( DIK_NUMPAD7 ) )
		pCamera->Roll(-kCameraRotationSpeed);
	//roll right
	if ( pInput->IsKeyDown( DIK_NUMPAD9 ) )
		pCamera->Roll(kCameraRotationSpeed);

	/********************************************/

	// clear the entire scene
	pDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255),1.0f,0);


	// begin the actual scene
	//pDevice->BeginScene();


	// Get and set the view matrix
	D3DXMATRIX viewMatrix;
	pCamera->CalculateViewMatrix(&viewMatrix);
	pDevice->SetTransform( D3DTS_VIEW, &viewMatrix );


	/********************************************/

	// Begin the scene
	pDevice->BeginScene();

	
	// rendering to go here
	// result - for catching error messages
	HRESULT rv;
	// specify where the data is going to come from
	rv = pDevice->SetStreamSource(
		0, // the stream number to set - just one for simplicity
		m_Cube.GetVertexBuffer(), // the vertex buffer to take the data from
		0, // offset in bytes from first vertex
		sizeof(CUSTOMVERTEX) // the stride of the vertices
	);
		// tell direct X the format of the custom vertex structure
	pDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	rv = pDevice->SetIndices(m_Cube.GetIndexBuffer());
	// draw the primitives (triangles) to Screen
	/*rv = pDevice->DrawPrimitive(
		D3DPT_TRIANGLELIST, //How to draw it
		0, //Which face in the list to start drawing from
		12 //Number of faces. Change this as you put in more vertices
	);*/
	rv = pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		8,
		0,
		12 );


	// End the scene
	pDevice->EndScene();

	pDevice->Present( NULL, NULL, NULL, NULL );

	//Set the frame time
	PreviousFrameTime = pctimer();
}

void DirectXWrapper::Cleanup()
{
    if( pDevice != NULL)
        pDevice->Release();
    if( pD3D != NULL)
        pD3D->Release();
}
