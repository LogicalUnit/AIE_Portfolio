// mathPractice.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "mathPractice.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];			// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];		// the main window class 

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

// initialise the global variables for TrueVision
CTVEngine		*pEngine;	// a pointer to the truevision engine
CTVScene		*pScene;	// a pointer to the scene 
CTVCamera		*pCamera;	// camera 
Object			*pObject;	// for math practice
bool			done = false;	// the game loop exit enabler
float			dt = 0;
HWND			hWnd	= NULL;


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
	LoadString(hInstance, IDC_MATHPRACTICE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	
    InitTV3D( hWnd );
	pObject = new Object( pScene );

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MATHPRACTICE));

	// Main loop
	while( ! done )
	{
		// Only update the renderer if we have focus to it.
		if ( GetFocus() == hWnd )
		{
			// get time since last update
			dt = pEngine->AccurateTimeElapsed();
			
			// clear the buffers
			pEngine->Clear(false);

			// set the flag to render everything by default
			pScene->RenderAll(true);

			pObject->Update(dt );

			// render it
			pEngine->RenderToScreen();		
		}
		else
		{
			// So we don't process the messages too many times if we are not rendering.
			Sleep( 100 );
		}

		// Main message
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MATHPRACTICE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MATHPRACTICE);
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
	   MessageBox( NULL, "Window Creation Failed", "ERROR", MB_OK | MB_ICONINFORMATION );
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
	// create and initialise the instance of the game engine
	pEngine = new CTVEngine();

	// set the search path and log file settings
	pEngine->SetSearchDirectory( "./" );
	pEngine->SetDebugFile( "Debug.txt" );
	pEngine->SetDebugMode( true, false );

	// register our key
	pEngine->SetLicenseFile( "Administrator.lic" );
	
	// Init the TV renderer and pass it the handle to the window
	if( !pEngine->Init3DWindowed( hwnd, true ) )
		return false;

	// set misc values for the TV engine
	pEngine->GetViewport()->SetAutoResize( true );
	pEngine->DisplayFPS( true );
	pEngine->SetAngleSystem( cTV_ANGLE_DEGREE );

	// Create the scene graph
	pScene = new CTVScene();

	// Create the camera system with default parametres
	pCamera = new CTVCamera();
	pCamera->SetViewFrustum(60.f, 1000.f, 1.f);
	
	//Set position and lookat
	pCamera->SetPosition( 0.f, 200.f, 400.f );
	pCamera->SetLookAt( 0.f, 0.f, 0.f);
	
	return true;
}
/***************************************/


/** CleanTV3D **************************/
void CleanTV3D()
{
	if( pObject )
	{
		delete( pObject );
		pObject = NULL;
	}

	if( pScene )
	{
		pScene->DestroyAllMeshes();
		delete( pScene );
		pScene = NULL;
	}

	if( pCamera )
	{
		delete( pCamera );
		pCamera = NULL;
	}

	if( pEngine )
	{
		delete( pEngine );
		pEngine = NULL;
	}
}
/**************************************/
