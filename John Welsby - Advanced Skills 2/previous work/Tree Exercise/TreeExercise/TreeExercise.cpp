// TreeExercise.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TreeExercise.h"
#include "Engine.h"
#include "Example.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// initialise the global variables for our window
HDC		hDC		= NULL;
HWND	hWnd	= NULL;
bool	done	= false;
float	dt;
Example	*pExample = NULL;


float camPositionX = 0.0;
float camPositionY = 10.0;
float camPositionZ = -100.0;
float camLookAtX = -0.069f;
float camLookAtY = 10.2f;
float camLookAtZ = -99.0f;
float camAngleX = 0.0f;
float camAngleY = 0.0f;
float camWalk = 0.0f;
float camStrafe = 0.0f;
float camUp = 0.0f;
float camSpeed = 1.0f;
float fanSpeed = 0.1f;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// ignore unused command line parameters
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// window message variable
	MSG msg;

	// attempt to create our render window - name, width, height, bit depth
	if( ! CreateTVRenderWindow( "TrueVision3D Tutorial", 800, 600, 16 ) )
	{
		//MessageBox( NULL, "Window Initialisation Failed", "Error", MB_OK | MB_ICONINFORMATION );
		return ( 0 );
	}

	// start our engine and load up everything
	InitTV3D( hWnd );

	pExample = new Example();

	// Main loop
	while( ! done )
	{
		// Only update the render if we have focus to it.
		if ( GetFocus() == hWnd )
		{
			delta = EngineManager::Instance()->GetEngine()->AccurateTimeElapsed() /1000 ;

			// clears the buffers
			EngineManager::Instance()->GetEngine()->Clear(false);

			Input();

			pExample->Render();

			// sets a flag to say render everything in the scene by default
			EngineManager::Instance()->GetScene()->RenderAll(true);

			
			DebugData();

			// now we actually render the scene
			EngineManager::Instance()->GetEngine()->RenderToScreen();

			// check for esc press
			/*if( pInput->IsKeyPressed( cTV_KEY_ESCAPE ) )
				done = true;*/
		}
		else
		{
			// So we don't process the messages too many times if we are not rendering.
			Sleep( 100 );
		}

		// Process normal window messages
		if ( PeekMessage( &msg, NULL, 0,0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	// Make sure we clean up after ourselves
	CleanTV3D();

	// exit to windows
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
/*******CreateTVRenderWindow************/
bool CreateTVRenderWindow( char * title,  int width, int height, int bits ) 
{
	// get the handle for the window
	hInst = GetModuleHandle( NULL );

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_TREEEXERCISE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= "TV3DWindow";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	// register the window
	if( ! ( RegisterClassEx( &wcex ) ) )
	{
		MessageBox( NULL, "Class Registration Failed", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;
	}

	// create the window
	if( ! ( hWnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,  "TV3DWindow", title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
		CW_USEDEFAULT,  width,  height,  NULL, NULL, hInst, NULL ) ) )
	{
		MessageBox( NULL, "Window Creation Failed", "ERROR", MB_OK | MB_ICONINFORMATION );
		return false;
	}

	// display the window
	ShowWindow( hWnd, SW_SHOW );
	SetForegroundWindow( hWnd );
	SetFocus( hWnd );

	return true;
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
   HWND hWnd;

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch ( uMsg )
	{
	case WM_CLOSE:
		{
			return(0);
		} 

	case WM_SIZE:								
		{
			return (0);							
		} 

	default: 
		break;
	}

	return ( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
}

bool InitTV3D( HWND hwnd )
{
	EngineManager::Instance()->Init(hwnd);

	return true;
}

void CleanTV3D()
{
	EngineManager::DestroyInstance();
}


void Input()
{
	CTVInputEngine *pInput = EngineManager::Instance()->GetInput();
	CTVCamera *pCamera = EngineManager::Instance()->GetCamera();

	bool justPressed = false;
	static bool currentDown = false;

	if (pInput->IsKeyPressed(cTV_KEY_ESCAPE))
		done = true;

	if (pInput->IsKeyPressed(cTV_KEY_W))
		camWalk = camSpeed;

	if (pInput->IsKeyPressed(cTV_KEY_S))
		camWalk = -camSpeed;

	if (pInput->IsKeyPressed(cTV_KEY_A))
		camStrafe = camSpeed;

	if (pInput->IsKeyPressed(cTV_KEY_D))
		camStrafe = -camSpeed;
	
	if (pInput->IsKeyPressed(cTV_KEY_Q))
		camUp = camSpeed;

	if (pInput->IsKeyPressed(cTV_KEY_Z))
		camUp = -camSpeed;

	if (pInput->IsKeyPressed(cTV_KEY_ESCAPE))
		done = true;

	if (pInput->IsKeyPressed(cTV_KEY_1) && !currentDown)
	{
		justPressed = true;
		currentDown = true;
	}

	if (!pInput->IsKeyPressed(cTV_KEY_1))
		currentDown = false;

	if (justPressed)
		pExample->KeyboardEvent(49, false);

	int tmpMouseX = 0, tmpMouseY = 0;
	pInput->GetMouseState(&tmpMouseX, &tmpMouseY);

	camAngleX = camAngleX - ((float)tmpMouseY / 100.0f);
    camAngleY = camAngleY - ((float)tmpMouseX / 100.0f);

	float fTimeElapsed = delta*1000;

	camPositionX = camPositionX + (cosf(camAngleY) * camWalk * fTimeElapsed) + (cosf(camAngleY + 3.141596f / 2.0f) * camStrafe * fTimeElapsed);
    camPositionZ = camPositionZ + (sinf(camAngleY) * camWalk * fTimeElapsed) + (sinf(camAngleY + 3.141596f / 2.0f) * camStrafe * fTimeElapsed);
	camPositionY = camPositionY + camUp * fTimeElapsed;
   
	camLookAtX = camPositionX + cosf(camAngleY);
    camLookAtY = camPositionY + tanf(camAngleX);
    camLookAtZ = camPositionZ + sinf(camAngleY);


	pCamera->SetPosition(camPositionX, camPositionY, camPositionZ);
	pCamera->SetLookAt(camLookAtX, camLookAtY, camLookAtZ);

	camStrafe = camWalk = camUp = 0.0f;	
}

void DebugData()
{

}