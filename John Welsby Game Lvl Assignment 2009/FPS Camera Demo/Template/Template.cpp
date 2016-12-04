// Template.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Template.h"

#include "JCWCameraSingleton.h"
#include "JCWInputSingleton.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;						// current instance
TCHAR szTitle[MAX_LOADSTRING];			// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];	// the main window class 

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

// initialise the global variables for our window
HWND		hWnd	= NULL;

/////////////////////////////////////////////////////////////////////////////
// initialise the global variables for TrueVision
CTVEngine			*pEngine;	// a pointer to the truevision engine
CTVScene			*pScene;	// a pointer to the scene 
JCWCameraSingleton	*pCamera;	// a pointer to the camera
JCWInputSingleton	*pInput;	// a pointer to all input

CTVMesh				*pBox;		// a pointer to a mesh

// input and camera variables 
float camPositionX = 0.0;
float camPositionY = 100.0;
float camPositionZ = 100.0;
float camLookAtX = 0.0;
float camLookAtY = 0.0;
float camLookAtZ = 0.0;
float camAngleX = 0.0;
float camAngleY = 0.0;

//We could have done this in many ways, but we added some smoothing to
//the movement se we need to declare two additional variables.
float camWalk = 0.0;
float camStrafe = 0.0;

// input handling function
void Input();

/////////////////////////////////////////////////////////////////////////////

bool	done = false;	// the game loop exit enabler
float	dt = 0;			// time elapsed

/////////////////////////////////////////////////////////////////////////////

// Forward declarations of functions included in this code module:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEMPLATE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	InitTV3D( hWnd );

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEMPLATE));

	// Main loop
	while( !done )
	{
		// Only update the renderer if we have focus to it.
		if ( GetFocus() == hWnd )
		{
			/////////////////////////////////////////////////////////////////////////////
			// get time since last update
			dt = pEngine->AccurateTimeElapsed();

			// call input
			Input();

			/////////////////////////////////////////////////////////////////////////////

			// clear the buffers
			pEngine->Clear(false);

			// set the flag to render everything by default
			pScene->RenderAll(true);

			// render it
			pEngine->RenderToScreen();		
		}
		else
		{
			// So we don't process the messages too many times if we are not rendering.
			Sleep( 100 );
		}

		// Window messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	// Make sure we clean up after ourselves
	CleanTV3D();

	return (int) msg.wParam;


}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEMPLATE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			// Make sure we clean up after ourselves
			done = true;
		
			PostQuitMessage( 0 );
			return(0);
		} break;

	case WM_SIZE:								
		{
			return 0;							
		} break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

}

/*** InitTV3D **************************/
bool InitTV3D( HWND hwnd )
{
	pEngine = new CTVEngine();

	pEngine->SetSearchDirectory( "./" );
	pEngine->SetDebugFile( "Debug.txt" );
	pEngine->SetDebugMode( true, false );

	// Init the TV renderer and pass it the handle to the window
	if( !pEngine->Init3DWindowed( hwnd, true ) )
		return false;

	// set misc values for the TV engine
	pEngine->GetViewport()->SetAutoResize( true );
	pEngine->DisplayFPS( true );
	pEngine->SetAngleSystem( cTV_ANGLE_DEGREE );

	pScene = new CTVScene();

	/////////////////////////////////////////////////////////////////////////////
	// camera
	pCamera = JCWCameraSingleton::GetInstance();
	pCamera->SetViewFrustum( 60.f, 1000.f);
	pCamera->SetPosition( 0.f, 100.f, 100.f );
	pCamera->SetLookAt( 0.f, 0.f, 0.f );

	// input
	//pInput = new CTVInputEngine();
	//pInput->Initialize( true, true );
	pInput = JCWInputSingleton::GetInstance();

	

	// Create a simple box so we have something for the camera to look at
	pBox = pScene->CreateMeshBuilder( "Box" );
	pBox->CreateBox( 64.f, 64.f, 64.f, false );
	pBox->SetPosition( 0.f, 0.f, 0.f );
	pBox->SetColor( cTV_COLORKEY_RED );
	pBox->SetLightingMode( cTV_LIGHTING_NORMAL );

	/////////////////////////////////////////////////////////////////////////////

	return true;
}

/** CleanTV3D **************************/
void CleanTV3D()
{
	/////////////////////////////////////////////////////////////////////////////
	if( pInput )
	{
		JCWInputSingleton::DeleteInstance();
		pInput = NULL;
	}

	if( pCamera )
	{
		JCWCameraSingleton::DeleteInstance();
		pCamera = NULL;
	}

	/////////////////////////////////////////////////////////////////////////////
	// pBox is destroyed by this call
	if( pScene )
	{
		pScene->DestroyAllMeshes();
		delete( pScene );
		pScene = NULL;
	}

	if( pEngine )
	{
		delete( pEngine );
		pEngine = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
//Input Handling
void Input()
{
    //Check if we pressed the UP arrow key, if so, then we are
    //walking forward.
    if(pInput->IsKeyPressed(cTV_KEY_UP))
        camWalk = 1;

    //If we are not walking forward, maybe we are walking backward
    //by using the DOWN arrow? If so, set walk speed to negative.
    if(pInput->IsKeyPressed(cTV_KEY_DOWN))
        camWalk = -1;

    //Check if we pressed the LEFT arrow key, if so, then strafe
    //on the left.
    if(pInput->IsKeyPressed(cTV_KEY_LEFT))
        camStrafe = 1;

    //If we are not strafing left, maybe we want to strafe to the
    //right, using the RIGHT arrow? If so, set strafe to negative.
    if(pInput->IsKeyPressed(cTV_KEY_RIGHT))
        camStrafe = -1;

    //Now, for the mouse input...
    int tmpMouseX = 0, tmpMouseY = 0;
    bool tmpMouseB1 = 0, tmpMouseB2 = 0, tmpMouseB3 = 0, tmpMouseB4 = 0;
    int tmpMouseScrollOld = 0, tmpMouseScrollNew = 0;

    //We pass the actual value of the mouse scroller to the variable
    //that holds the old mouse scroller value.
    tmpMouseScrollOld = tmpMouseScrollNew;

    //By using GetMouseState, we get the movement of the mouse
    //with the speed of the movement. The fastest the mouse movement
    //will be, the higher will be the return.
    pInput->GetMouseState(&tmpMouseX, &tmpMouseY, &tmpMouseB1, &tmpMouseB2, &tmpMouseB3, &tmpMouseB4, &tmpMouseScrollNew);

    //From the mouse return values, we update the camera angles
    //by adding or substracting the mouse return value.
    camAngleX = camAngleX - ((float)tmpMouseY / 100.0f);
    camAngleY = camAngleY - ((float)tmpMouseX / 100.0f);

    //We will add a simple check, so we can't look up at more
    //than 80 degrees nor down than -80 degrees.
    if(camAngleX > 1.3f) 
		camAngleX = 1.3f;

    if(camAngleX < -1.3f) 
		camAngleX = -1.3f;

    //Okay, now for the smothing of the movement... We checked
    //above if we were pressing a key. If so, then we updated the
    //movement variable to 1 (positive or negative). Here, we
    //lower this value until it get to 0. This method give us a
    //smoother camera movement. We start by updating the forward
    //and backward (walk) movement.

    if(camWalk > 0)
	{
        camWalk = camWalk - 0.05f;
        if(camWalk < 0) camWalk = 0;
	}
    if(camWalk < 0)
	{
        camWalk = camWalk + 0.05f;
        if(camWalk > 0) camWalk = 0;
	}

    //Now, we update the left and right (strafe) movement.
    if(camStrafe > 0)
	{
        camStrafe = camStrafe - 0.05f;
        if(camStrafe < 0) camStrafe = 0;
	}
    if(camStrafe < 0)
	{
        camStrafe = camStrafe + 0.05f;
        if(camStrafe > 0) camStrafe = 0;
	}
	
	float fTimeElapsed = dt;
   
	//Update the vectors using the angles and positions.
    camPositionX = camPositionX + (cosf(camAngleY) * camWalk * fTimeElapsed) + (cosf(camAngleY + 3.141596f / 2.0f) * camStrafe * fTimeElapsed);
    camPositionZ = camPositionZ + (sinf(camAngleY) * camWalk * fTimeElapsed) + (sinf(camAngleY + 3.141596f / 2.0f) * camStrafe * fTimeElapsed);
   
	camLookAtX = camPositionX + cosf(camAngleY);
    camLookAtY = camPositionY + tanf(camAngleX);
    camLookAtZ = camPositionZ + sinf(camAngleY);

    //With the new values of the camera vectors (position and
    //look at), we update the scene's camera.
    pScene->SetCamera(camPositionX, camPositionY, camPositionZ, camLookAtX, camLookAtY, camLookAtZ);

	if(pInput->IsKeyPressed(cTV_KEY_ESCAPE))		//Check if ESCAPE has been pressed.
	{		
		done = true;
	}
}
/////////////////////////////////////////////////////////////////////////////