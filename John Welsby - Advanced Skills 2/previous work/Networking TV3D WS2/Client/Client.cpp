#include "StdAfx.h"
#include <sstream>
#include <string>

/*** InitTV3D **************************/
bool InitTV3D(  )
{	
	if( ! CreateTVRenderWindow( "TrueVision3D Game client", 800, 600, 16 ) )
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

	g_Network.InitializeWinsock ();
	g_Network.OpenClientSocket ( hWnd, strIPAddress, iPort );

	pGameTask = new GameTask( pEngine, g_Network );

////////////////////////////////////////////////
		
////////////////////////////////////////////////////
	return true;
}
/***************************************/

/** CleanTV3D **************************/
void CleanTV3D()
{
	///////////////////////////////////////////////////
	g_Network.Disconnect ();
	g_Network.TerminateWinsock ();

	UnregisterClass( "TV3DClient", hInstance );

	/////////////////////////////////////////////////////

	
	if( pEngine )
	{
		delete( pEngine );
		pEngine = NULL;
	}
}

/*** WinMain ***************************/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevinstance, LPSTR lpCmdLine, int nShowCmd )
{
	// declare necessary variables
	MSG msg;

	if( !InitTV3D( ) )
	{
		MessageBox( NULL, "Window Initialisation Failed", "Error", MB_OK | MB_ICONINFORMATION );
		CleanTV3D();
		return -1;
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
		if( PeekMessage( &msg,NULL,0,0,PM_REMOVE ) )
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

	return (int)msg.wParam;
}

/***************************************/


/*** The Event Handler *****************/
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
		case Network::WM_NETWORK_MESSAGE:
		{
			try
			{
				NetMessage *message = NULL;

				message = g_Network.ProcessMessage ( wParam, lParam );
			
				if( message )
				{
					if( message->ID == 602 )
					{
						PositionMessage *posMess = ( PositionMessage * )message;
						pGameTask->PositionUpdate( posMess );

						char buff[128];
						sprintf_s( buff, 128,  "Position: %f %f %f", posMess->posx, posMess->posy, posMess->posz );
						PrintToOutput ( buff );					
					}
					else if( message->ID == 601 )
					{
						PrintToOutput ( message->message );

					}
					else if( message->ID == 603 )
					{
						PrintToOutput ( message->message );
					}
					else
					{

					}
				}
			}
			catch ( std::exception e )
			{
				pEngine->AddToLog( "exception");
			}
			return 0;
		}
		break;

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
			break;
	}

	return ( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
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
LRESULT CALLBACK NetworkOptProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	//char	cbuf[256];

    switch( message )										// Switch on all of the dialogs messages
    {
		
		case WM_COMMAND:
			
            break;

        case WM_CLOSE:
			
			break;

		case WM_DESTROY:
			break;
    }

    return FALSE;
}

void	PrintToOutput (  const char* str )
{
	/*char buff[128];
	strcpy_s( buff, 128, str );
	pEngine->AddToLog( buff );*/

	//BufferedOutput::Instance()->ClearBuf();
	BufferedOutput::Instance()->AddText(str);
	BufferedOutput::Instance()->AddText("\n");
}
