#include "StdAfx.h"



/*** InitTV3D **************************/
bool InitServer( HWND hwnd )
{
	pEngine = new CTVEngine();

	pEngine->SetSearchDirectory( "./" );
	pEngine->SetDebugFile( "Debug.txt" );
	pEngine->SetDebugMode( true, false );


	////////////////////////////////////////////////////
	g_Network.InitializeWinsock ();

	if( ! CreateServerWindow( "Dedicated Server", 400, 400 ) )
	{
		MessageBox( NULL, "Window Initialisation Failed", "Error", MB_OK | MB_ICONINFORMATION );
		return ( 0 );
	}

	g_Network.OpenServerSocket ( hWnd, iPort );
	
	return true;
}
/***************************************/
/** CreateTVRenderWindow ***************/
bool CreateServerWindow( char* title, int width, int height )
{
	// variable for the windows class
	WNDCLASSEX wc = {0}; // Our extended window class

	// define the window here
	hInstance = GetModuleHandle( NULL );

	wc.cbSize			= sizeof( WNDCLASSEX );
	wc.style			= CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon( NULL, IDI_WINLOGO );
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "TV3DServer";
	wc.hIconSm			= LoadIcon( NULL, IDI_APPLICATION );

	if( ! ( RegisterClassEx( &wc ) ) )
	{
		MessageBox( NULL, "Class Registration Failed", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;
	}

	hWnd = CreateWindowEx( WS_EX_APPWINDOW, 
							"TV3DServer", 
							title, 
							WS_DLGFRAME|WS_SYSMENU|WS_CAPTION|WS_OVERLAPPED,
							CW_USEDEFAULT,	
							CW_USEDEFAULT, 
							width, 
							height, 
							NULL, 
							NULL, 
							hInstance, 
							NULL );
	if( !hWnd )
	{
		MessageBox( NULL, "Window Creation Failed", "ERROR", MB_OK | MB_ICONINFORMATION );
		return false;
	}

	ShowWindow( hWnd, SW_SHOW );
	SetForegroundWindow( hWnd );
	SetFocus( hWnd );
	UpdateWindow( hWnd );

	return true;
}
/***************************************/
/** CleanTV3D **************************/
void CleanServer()
{
	///////////////////////////////////////////////////
	g_Network.Disconnect ();
	g_Network.TerminateWinsock ();

	UnregisterClass( "TV3DServer", hInstance );

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

	// Try to initialize everything
	try
	{
		InitServer( hWnd );
		SetWindowText ( hWnd, "Dedicated Server" );
	}
	catch ( std::exception e )
	{
		MessageBox ( GetActiveWindow(), e.what(), "Fatal Error during initalization", MB_OK|MB_ICONERROR );
		CleanServer();
		return -1;
	}

	// Main loop
	while( ! bIsDone )
	{	
		float fTickCount = pEngine->AccurateTimeElapsed();


		// Process normal messages
		// Scan for a new Win32 message
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			// If it is a quit message, then kill this loop
			if(msg.message == WM_QUIT)
				break;

			// We have to cheat and seek out the release of VK_ENTER
			// before the window has a chance to override it first!
			if(msg.message == WM_KEYUP && g_hWndInputbox )
			{
				if ( LOWORD(msg.wParam) == VK_RETURN )
				{
					int length = GetWindowTextLength(g_hWndInputbox);
					char *input = new char[length+1];
					GetWindowText ( g_hWndInputbox, input, (length+1) );

					SetWindowText ( g_hWndInputbox, "\0" );

					std::string	msg  = std::string("[");
								msg += std::string(strUsername);
								msg += std::string("]\t");
								msg += input;
				
					PrintToOutput ( msg.c_str() );

					NetMessage nwMess;
					strcpy_s( nwMess.message, sizeof(nwMess.message), msg.c_str() );
					g_Network.Send ( (char *)&nwMess, sizeof( NetMessage ) );
					
					if ( strcmp(input, "quit") == 0 )
						bIsDone = true;
						
					delete input;
				}
			}
			
			// Otherwise, let the WndProc function take care of it.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Put your game thread here


			Sleep ( 1 );
		}
	}

	// Make sure we clean up after ourselves
	CleanServer();

	return (int)msg.wParam;
}




void	PrintToOutput ( const char* str )
{
	if ( !g_hWndEditbox ) return;

	int oldlength = GetWindowTextLength( g_hWndEditbox );

	int newlength = oldlength + (int)strlen(str) + 2;

	char *oldstr = new char[oldlength+1];
	GetWindowText ( g_hWndEditbox, oldstr, oldlength+1 );

	char *bigstringbuffer = new char[newlength+1];
	sprintf_s ( bigstringbuffer, newlength+1, "%s\r\n%s", str, oldstr );

	SetWindowText ( g_hWndEditbox, bigstringbuffer );

	delete bigstringbuffer;
}

/*** The Event Handler *****************/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	// Do stuff depending on the "message"
    switch(message)
    {
        case WM_CREATE:
		{
			if ( hwnd == g_hWndInputbox || hwnd == g_hWndEditbox ) return 0;

			int		iInputHeight = 42;
			RECT	rect;
			GetClientRect(hwnd,&rect);
			g_hWndEditbox = CreateWindow("EDIT",NULL,WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL |
									ES_AUTOVSCROLL | ES_LEFT | ES_MULTILINE | ES_NOHIDESEL |
									WS_VSCROLL | WS_HSCROLL | WS_BORDER | ES_READONLY,
									0, 0, rect.right, rect.bottom-iInputHeight, hwnd, (HMENU)0,
									((LPCREATESTRUCT)lparam)->hInstance, NULL);

			g_hWndInputbox = CreateWindow ("EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL |
									ES_LEFT | ES_NOHIDESEL | EN_UPDATE | WS_HSCROLL | WS_BORDER,
									0, rect.bottom-iInputHeight, rect.right, iInputHeight, hwnd, (HMENU)0,
									((LPCREATESTRUCT)lparam)->hInstance, NULL);

			return 0;
		}

		case Network::WM_NETWORK_MESSAGE:
		{
			try
			{
				NetMessage *netmessage = NULL;

				netmessage = g_Network.ProcessMessage ( wparam, lparam );

				if( netmessage )
				{
                    if( netmessage->ID == 602 )
					{
						PositionMessage *posMess = ( PositionMessage * )netmessage;
		
						char buff[128];
						sprintf_s( buff, 128, "Position: %f %f %f", posMess->posx, posMess->posy, posMess->posy );
						PrintToOutput ( buff );					
					
						PositionMessage msg;
						msg.posx = posMess->posx;
						msg.posy = posMess->posy;
						msg.posz = posMess->posz;
						strcpy_s( msg.message, sizeof( "Position message recieved" ), "Position message recieved" );
                        g_Network.Send( (char *)&msg, sizeof( PositionMessage ) );
					}
					else if( netmessage->ID == 601)
					{
						PrintToOutput ( netmessage->message );

						//Echo messages to all the clients
						NetMessage msg;
						strcpy_s(msg.message, sizeof(netmessage->message), netmessage->message);
						g_Network.Send( (char*)&msg, sizeof(NetMessage) );
					}
					else if ( netmessage->ID == 603)
					{
						TauntMessage msg;
						strcpy_s(msg.message, sizeof(netmessage->message), netmessage->message);
						g_Network.Send( (char*)&msg, sizeof(NetMessage) );
					}
					else
					{

					}
				}
			}
			catch ( std::exception e )
			{
				PrintToOutput ( e.what() );
			}
			return 0;
		}

        case WM_CLOSE:
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}

/***************************************/


