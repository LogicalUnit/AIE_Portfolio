// TV3DCreating.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Engine.h"
#include "Template.h"
#include "Level.h"
#include "Actor.h"
#include "Player.h"

#include "WaypointGenerator.h"

#include "Intelligence.h"

// Global Variables:
HINSTANCE hInst;								// current instance
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

// initialise the global variables for our window
HDC		hDC		= NULL;
HWND	hWnd	= NULL;

bool done = false;

//Actor MyActor;

// Main entry point to the program
int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// ignore unused command line parameters
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// window message variable
	MSG msg;
	

	// attempt to create our render window - name, width, height, bit depth
	if( ! CreateTVRenderWindow( "Assignment2 Client", 800, 600, 16 ) )
	{
		MessageBox( NULL, "Window Initialisation Failed", "Error", MB_OK | MB_ICONINFORMATION );
		return ( 0 );
	}

	// start our engine and load up everything
	InitTV3D( hWnd );
	
	//WaypointGenerator wpGen("waypoints.txt");

	// Main loop
	while( ! done )
	{
		// Only update the render if we have focus to it.
		if ( GetFocus() == hWnd )
		{
			// check for esc press
			if( EngineManager::Instance()->GetInput()->IsKeyPressed( cTV_KEY_ESCAPE ) )
				done = true;

			float dt = EngineManager::Instance()->GetEngine()->AccurateTimeElapsed();

			// clears the buffers
			EngineManager::Instance()->GetEngine()->Clear(false);

			Input(dt); 

			
			/*static bool oDown = false;
			if(EngineManager::Instance()->GetInput()->IsKeyPressed( cTV_KEY_O ) && !oDown)
			{
				wpGen.AddWaypoint( EngineManager::Instance()->GetPlayerCharacter()->GetPosition() );
				oDown = true;
			}
			if(!EngineManager::Instance()->GetInput()->IsKeyPressed( cTV_KEY_O ))
				oDown = false;*/


			Player::UpdatePlayers(dt);

			EngineManager::Instance()->NetworkUpdate();

			
			// sets a flag to say render everything in the scene by default
			EngineManager::Instance()->GetScene()->RenderAll(true);
			
			PrintHUDText();			

			// now we actually render the scene
			EngineManager::Instance()->GetEngine()->RenderToScreen();
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
/***************************************/



/*** InitTV3D **************************/
bool InitTV3D( HWND hwnd )
{

	EngineManager::Instance()->Init(hwnd);

	//MyActor.LoadModel("actor", "mgolden/AlienLowPoly.tva", EngineManager::Instance()->GetScene() );

	//EngineManager::Instance()->GetCamera()->ChaseCameraActor(
	

	std::vector<ObjectData> Level1List;

	//tiny xml - load file
	LoadXML( "tvmexport.xml", Level1List );

	// Load the scene objects 
	if( !LoadScene( Level1List ) )
	{
		return false;
	}

	//can empty the list now - for memory saving
	if( !Level1List.empty() )
	{
		// delete all
		Level1List.erase( Level1List.begin(), Level1List.end() );		
	}

	SetupLights();

	// whew - all is good
	return true;
}
/***************************************/


/** CleanTV3D **************************/
void CleanTV3D()
{
	//if( !Level1List.empty() )
	//{
	//	// delete all
	//	Level1List.erase( Level1List.begin(), Level1List.end() );		
	//}

	EngineManager::DestroyInstance();
}
/***************************************/


/*** The window Event Handler *****************/
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_CLOSE:
		{
			// Make sure we clean up after ourselves
			done = true;
			PostQuitMessage( 0 );
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
/***************************************/

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
	wcex.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_TEMPLATE));
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
