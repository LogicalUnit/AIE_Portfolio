// RakNetTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RakNetTest.h"

#define MAX_LOADSTRING 100


/*** WinMain ***************************/
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// declare necessary variables
	MSG msg;

 	if( !InitTV3D() )
	{
		MessageBox( NULL, "Window Initialisation Failed", "Error", MB_OK | MB_ICONINFORMATION );
		bIsDone = true;
	}
	
	// Main loop
	while( ! bIsDone )
	{
		// Put your game thread here
		// Only update the render if we have focus to it.
		if ( GetFocus() == hWnd )
		{
			// Get the tickcount so updates are more smooth 
			float dt = pEngine->AccurateTimeElapsed();

			if( !pGameTask->Update( dt ) )
				bIsDone = true;
		}
		else
		{
            Sleep ( 100 );
		}
			
		// Process normal messages
		// Scan for a new Win32 message
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			// If it is a quit message, then kill this loop
			if(msg.message == WM_QUIT)
				break;

			/// Otherwise, let the WndProc function take care of it.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}		
	}		

	// Make sure we clean up after ourselves
	CleanTV3D();
	UnregisterClass( "TV3DWindow", hInstance );
	return (int) msg.wParam;
}

/*** InitTV3D **************************/
bool InitTV3D( )
{	
	if( ! CreateTVRenderWindow( "TrueVision3D RakNet Test", 800, 600, 16 ) )
	{
		MessageBox( NULL, "Window Initialisation Failed", "Error", MB_OK | MB_ICONINFORMATION );
		return ( 0 );
	}

	pEngine = new CTVEngine();

	pEngine->SetSearchDirectory( "./" );
	pEngine->SetDebugFile( "Debug.txt" );
	pEngine->SetDebugMode( true, false );

	// Init the TV renderer and pass it the handle to the window
	if( !pEngine->Init3DWindowed( hWnd, true ) )
		return false;

	// set misc values for the TV engine
	pEngine->GetViewport()->SetAutoResize( true );
	pEngine->DisplayFPS( true );
	pEngine->SetAngleSystem( cTV_ANGLE_DEGREE );

////////////////////////////////////////////////
	pGameTask = new GameTask( pEngine );



////////////////////////////////////////////////
		
////////////////////////////////////////////////////
	return true;
}
/***************************************/

/** CleanTV3D **************************/
void CleanTV3D()
{
	///////////////////////////////////////////////////
	

	/////////////////////////////////////////////////////

	
	if( pEngine )
	{
		delete( pEngine );
		pEngine = NULL;
	}
}
/***************************************/

/** CreateTVRenderWindow ***************/
bool CreateTVRenderWindow( char* title, int width, int height, int bits )
{
	// variable for the windows class
	WNDCLASSEX wc;

	// define the window here
	hInstance = GetModuleHandle( NULL );

	wc.cbSize			= sizeof( WNDCLASSEX );
	wc.style			= CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon( NULL, IDI_WINLOGO );
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "TV3DWindow";
	wc.hIconSm			= LoadIcon( NULL, IDI_APPLICATION );

	if( ! ( RegisterClassEx( &wc ) ) )
	{
		MessageBox( NULL, "Class Registration Failed", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;
	}

	if( ! ( hWnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, "TV3DWindow", title,	WS_OVERLAPPEDWINDOW,
		                           CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL ) ) )
	{
		MessageBox( NULL, "Window Creation Failed", "ERROR", MB_OK | MB_ICONINFORMATION );
		return false;
	}

	ShowWindow( hWnd, SW_SHOW );
	SetForegroundWindow( hWnd );
	SetFocus( hWnd );

	return true;
}
/***************************************/
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_CLOSE:
		{
			// Make sure we clean up after ourselves
			bIsDone = true;
			CleanTV3D();
			PostQuitMessage( 0 );
			return(0);
		} 
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/***************************************/
void	PrintToOutput (  const char* str )
{
	char buff[128];
	strcpy_s( buff, 128, str );

	pEngine->AddToLog( buff );

}