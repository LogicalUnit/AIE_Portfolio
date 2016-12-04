#include "stdafx.h"

// Defined in Main.CPP
extern char	strUsername[128];

inline int Min ( int a, int b )
{
	return ( a < b ) ? a : b;
}

void PrintToOutput ( const char* );


Network::Network ()
{
	bConnected = false;
	pReadBuffer = new char[GetMaxMessageLength()];
}

Network::~Network ()
{
	
}


/**
 * Initialize Winsock2
 */
void Network::InitializeWinsock ()
{
	// WSAStartup needs to be called prior to any other Winsock functions
	int	error = WSAStartup ( 0x0202, &wsaData );

	// If there was an error
	if ( error )
	{
		// Throw an exception stating the error
		char	errormsg[512];
		sprintf_s ( errormsg, 512, "WSAStartup failed - %s", DescribeError(GetLastError()) );
		throw std::exception ( errormsg );
	}

	// If we didnt get winsock version 2.2, then throw an exception
	if ( wsaData.wVersion != 0x0202 )
		throw std::exception ( "WSAStartup failed - winsock isnt version 2");
}

/**
 * Terminate Winsock2
 */
void Network::TerminateWinsock ()
{
	bConnected = false;
	WSACleanup ();
}

/**
 * This function returns a string describing (or at least, the code) the given error.
 */
const char*	Network::DescribeError	( int winsock_errorcode )
{
	switch ( winsock_errorcode )
	{
	case WSAEINTR:
		return "WSAEINTR";

	case WSAEBADF:
		return "WSAEBADF";

	case WSAEACCES:
		return "WSAEACCESS";

	case WSAEFAULT:
		return "WSAEFAULT";

	case WSAEINVAL:
		return "WSAEINVAL";

	case WSAEMFILE:
		return "WSAEMFILE";

	case WSAEWOULDBLOCK:
		return "WSAEWOULDBLOCK - busy";

	case WSAEINPROGRESS:
		return "WSAEINPROGRESS - busy";

	case WSAEALREADY:
		return "WSAEALREADY";

	case WSAENOTSOCK:
		return "WSAENOTSOCK - not a valid socket";

	case WSAEDESTADDRREQ:
		return "WSAEDESTADDRREQ";

	case WSAEMSGSIZE:
		return "WSAEMSGSIZE";

	case WSAEPROTOTYPE:
		return "WSAEPROTOTYPE";

	case WSAENOPROTOOPT:
		return "WSAENOPROTOOPT";

	case WSAEPROTONOSUPPORT:
		return "WSAEPROTONOSUPPORT";

	case WSAESOCKTNOSUPPORT:
		return "WSAESOCKTNOSUPPORT";

	case WSAEOPNOTSUPP:
		return "WSAEOPNOTSUPP";

	case WSAEPFNOSUPPORT:
		return "WSAEPFNOSUPPORT";

	case WSAEAFNOSUPPORT:
		return "WSAEAFNOSUPPORT";

	case WSAEADDRINUSE:
		return "WSAEADDRINUSE - Address in use";

	case WSAEADDRNOTAVAIL:
		return "WSAEADDRNOTAVAIL - Address not available";

	case WSAENETDOWN:
		return "WSAENETDOWN - Underlying failure / Network down";

	case WSAENETUNREACH:
		return "WSAENETUNREACH - Network unreachable";

	case WSAENETRESET:
		return "WSAENETRESET - Network was reset";

	case WSAECONNABORTED:
		return "WSAECONNABORTED - Connection aborted";

	case WSAECONNRESET:
		return "WSAECONNRESET - Connection was reset";

	case WSAENOBUFS:
		return "WSAENOBUFS";

	case WSAEISCONN:
		return "WSAEISCONN";

	case WSAENOTCONN:
		return "WSAENOTCONN";

	case WSAESHUTDOWN:
		return "WSAESHUTDOWN";

	case WSAETOOMANYREFS:
		return "WSAETOOMANYREFS";

	case WSAETIMEDOUT:
		return "WSAETIMEDOUT - Connection timed out";

	case WSAELOOP:
		return "WSAELOOP";

	case WSAENAMETOOLONG:
		return "WSAENAMETOOLONG - Name too long";

	case WSAEHOSTDOWN:
		return "WSAEHOSTDOWN - Host is down";

	case WSAEHOSTUNREACH:
		return "WSAEHOSTUNREACH - Host is unreachable";

	case WSASYSNOTREADY:
		return "WSASYSNOTREADY - Network subsystem is unusable at this time";

	case WSAVERNOTSUPPORTED:
		return "WSAVERNOTSUPPORTED - Winsock DLL cannot support this program";

	case WSANOTINITIALISED:
		return "WSANOTINITIALISED - Winsock has not been initialized yet";

	case WSAEDISCON:
		return "WSAEDISCON - remote connection has disconnected";

	case WSA_OPERATION_ABORTED:
		return "WSA_OPERATION_ABORTED";

	case WSAHOST_NOT_FOUND:
		return "WSAHOST_NOT_FOUND";

	case WSATRY_AGAIN:
		return "WSATRY_AGAIN"; // you no win this time

	case WSANO_RECOVERY:
		return "WSANO_RECOVERY";

	case WSANO_DATA:
		return "WSANO_DATA";

	default:
		return "WSAError";
	}
}


void	Network::OpenServerSocket ( HWND wnd, int port )
{
	if ( bConnected )
		throw std::exception ( "Cannot open server socket, socket is already connected!" );

	// Retain handle to window (used for Asynchonrous sockets)
	hWnd = wnd;

	// Mark as server socket
	bServerSocket = true;

	// Set socket
	mysocket = socket (	AF_INET,		// Internet adress family
						SOCK_STREAM,	// Use a streaming socket (TCP/IP)
						0 );			// Let winsock pick the correct protocol settings

	// Check to see that we did aquire a socket
	if ( mysocket == INVALID_SOCKET )
	{
		// Throw an exception stating the error
		char	errormsg[512];
		sprintf_s ( errormsg, 512, "Cannot open server socket - %s", DescribeError(GetLastError()) );
		throw std::exception ( errormsg );
	}

	// Now we bind the socket (only applies to server sockets)
	socketaddress.sin_family = AF_INET;					// Internet address family, TCP/IP
	socketaddress.sin_port = htons(port);				// This specifies which port we are using
	socketaddress.sin_addr.s_addr = htonl(INADDR_ANY);	// No destination, accept from anywhere

	// Bind and check that it worked.
	if ( bind ( mysocket, (LPSOCKADDR)&socketaddress, sizeof(socketaddress) ) == SOCKET_ERROR )
	{
		// Throw an exception stating the error
		char	errormsg[512];
		sprintf_s ( errormsg, 512, "Cannot bind server socket - %s", DescribeError(GetLastError()) );
		throw std::exception ( errormsg );
	}

	// Set the socket to Asynchronous mode.
	//
	// Async sockets (non-blocking sockets) merge with the windows message pump,
	// so that we are notified when a networking message is present, rather than
	// us looking for messages.
	if ( WSAAsyncSelect(mysocket,				// our socket
						 hWnd,					// our handle to the window that will receive our messages
						 WM_NETWORK_MESSAGE,	// the message code that will be used to identify Winsock messages
						 (FD_READ	|	// We want to handle 'receive' messages
						  FD_CONNECT|	// We want to handle 'connect' messages
						  FD_CLOSE  |	// We want to handle 'disconnect' messages
						  FD_ACCEPT)	// We want to handle 'accept' messages (when a client connects to us).
						  ) == SOCKET_ERROR )	
	{
		// Throw an exception stating the error
		char	errormsg[512];
		sprintf_s ( errormsg, 512, "Cannot set asynchronous server socket - %s", DescribeError(GetLastError()) );
		throw std::exception ( errormsg );
	}

	// Begin listening
	int		iBacklog = 5;
	if ( listen ( mysocket, iBacklog ) == SOCKET_ERROR )
	{
		// Throw an exception stating the error
		char	errormsg[512];
		sprintf_s ( errormsg, 512, "Cannot begin listening on server socket - %s", DescribeError(GetLastError()) );
		throw std::exception ( errormsg );
	}

	// Mark as connected
	bConnected = true;
}




void Network::OpenClientSocket ( HWND wnd, const char *addr, int port )
{
	if ( bConnected )
		throw std::exception ( "Cannot open client socket, socket is already connected!" );

	// Retain handle to window (used for Asynchonrous sockets)
	hWnd = wnd;

	// Mark as client socket
	bServerSocket = false;

	// Set socket
	mysocket = socket (	AF_INET,		// Internet adress family
						SOCK_STREAM,	// Use a streaming socket (TCP/IP)
						0 );			// Let winsock pick the correct protocol settings

	// Check to see that we did aquire a socket
	if ( mysocket == INVALID_SOCKET )
	{
		// Throw an exception stating the error
		char	errormsg[512];
		sprintf_s ( errormsg, 512, "Cannot open client socket - %s", DescribeError(GetLastError()) );
		throw std::exception ( errormsg );
	}

	// Now we try to connect to the server socket
	socketaddress.sin_family = AF_INET;					// Internet address family, TCP/IP
	socketaddress.sin_port = htons(port);				// This specifies which port we are using
	socketaddress.sin_addr.s_addr = inet_addr(addr);	// This specifies the IP Address. 
														// inet_addr filters the address down to a long int.
	
	// See if a hostname was given instead of an IP address
	if ( socketaddress.sin_addr.s_addr == INADDR_NONE)
	{
		hostent *host = gethostbyname (addr);
		if ( host == 0 )
		{
			// Throw an exception stating the error
			throw std::exception ( "Cannot open client socket - unable to resolve hostname/ip" );
		}

		// Retreive ip address from host entity
		socketaddress.sin_addr.s_addr = *((u_long*)host->h_addr_list[0]);
	}


	// try to connect
	if ( connect ( mysocket, (LPSOCKADDR)&socketaddress, sizeof(socketaddress) ) == SOCKET_ERROR )
	{
		// Throw an exception stating the error
		char	errormsg[512];
		sprintf_s ( errormsg, 512, "Cannot connect client to server socket at %s - %s", addr, DescribeError(GetLastError()) );
		throw std::exception ( errormsg );
	}

	// Set the socket to Asynchronous mode.
	//
	// Async sockets (non-blocking sockets) merge with the windows message pump,
	// so that we are notified when a networking message is present, rather than
	// us looking for messages.
	if ( WSAAsyncSelect(mysocket,				// our socket
						 hWnd,					// our handle to the window that will receive our messages
						 WM_NETWORK_MESSAGE,	// the message code that will be used to identify Winsock messages
						 (FD_READ	|	// We want to handle 'receive' messages
						  FD_CLOSE) ) == SOCKET_ERROR )	// We want to handle 'disconnect' messages
	{
		// Throw an exception stating the error
		char	errormsg[512];
		sprintf_s ( errormsg, 512, "Cannot set asynchronous client socket - %s", DescribeError(GetLastError()) );
		throw std::exception ( errormsg );
	}

	// Mark as connected
	bConnected = true;

	// Send an message
	NetMessage msg;
	sprintf_s ( msg.message, sizeof(msg.message), "--> %s just entered the room...", strUsername );
	Send ( (char*)&msg, sizeof(msg) );
}

void	Network::Disconnect	( )
{
	if ( bConnected )
	{
		// Send an message
		NetMessage msg;
		sprintf_s ( msg.message, sizeof(msg.message), "--> %s just left the room...", strUsername );
		Send ( (char*)&msg, sizeof(msg) );

		// Should probably send a notification to clients or server that we are disconnecting
		for ( std::list<Client>::iterator it = clientlist.begin(); it != clientlist.end(); it++ )
		{
			shutdown	( it->clientsocket, SD_BOTH ); // Cannot receive or send anymore
			closesocket	( it->clientsocket );			// Close it!
		}

		shutdown	( mysocket, SD_BOTH ); // Cannot receive or send anymore
		closesocket	( mysocket );			// Close it!

		// Mark as disconnected
		bConnected = false;
	}
}

NetMessage	*Network::ProcessMessage ( WPARAM wParam, LPARAM lParam )
{
	NetMessage *newMessage = NULL;

	// check if it was an error message
	// WSAGETSELECTERROR is a windows macro
	if ( WSAGETSELECTERROR(lParam) )
	{
		char	errormsg[512];
		sprintf_s ( errormsg, 512, "*NetworkError* %s", DescribeError(GetLastError()) );
		throw std::exception ( errormsg );
	}
	
	// check what type of network message it is
	// WSAGETSELECTEVENT is a windows macro
	switch ( WSAGETSELECTEVENT(lParam) )
	{
	case FD_READ:
		{
			if ( pReadBuffer )
			{
				if ( recv ( (SOCKET)wParam, pReadBuffer, GetMaxMessageLength(), 0 ) == SOCKET_ERROR )
				{
					// If we fail to receive, just ignore it for now...
				}
			}
			
			// we have a message and it is not empty
			if ( pReadBuffer && pReadBuffer[0] )
			{
				newMessage = ( NetMessage * )pReadBuffer;
			}
		}
		break;

	case FD_CONNECT:
		{
			PrintToOutput ( "* FD_CONNECT received *" );
		}
		break;

	case FD_CLOSE:
		{
			PrintToOutput ( "* FD_CLOSE received *" );
		}
		break;

	case FD_ACCEPT:
		{
			PrintToOutput ( "* FD_ACCEPT received *" );

			clientlist.push_back ( Client() );
			int addrsize = sizeof(clientlist.back().clientaddress);

			clientlist.back().clientsocket = accept ( mysocket, (LPSOCKADDR)&clientlist.back().clientaddress, &addrsize );

			if ( clientlist.back().clientsocket == INVALID_SOCKET )
			{
				clientlist.pop_back();

				// Throw an exception stating the error
				char	errormsg[512];
				sprintf_s ( errormsg, 512, "Cannot set asynchronous client socket - %s", DescribeError(GetLastError()) );
				throw std::exception ( errormsg );
			}
		}
		break;
	}
	return newMessage;
}

void	Network::Send( const char* buffer, size_t length )
{
	// Override length to prevent buffer overflow on server end.
	// max message length is 512
	length = Min ( (int)length, GetMaxMessageLength() );

	if ( IsClient() )
	{
		// Just send to our socket
		send ( mysocket, buffer, (int)length, 0 );
	}
	else if ( IsServer() )
	{
		// Send to every user-socket
		for ( std::list<Client>::iterator it = clientlist.begin(); it != clientlist.end(); it++ )
		{
			send ( it->clientsocket, buffer, (int)length, 0 );
		}
	}
}

void	Network::Send			( const char* buffer, size_t length, SOCKET who, SendRule rule )
{
	if ( rule == EXCEPT_GIVEN_SOCKET )
	{
		for ( std::list<Client>::iterator it = clientlist.begin(); it != clientlist.end(); it++ )
		{
			if ( it->clientsocket != who )	
				send ( it->clientsocket, buffer, (int)length, 0 );
		}
	}
	else // rule == ONLY_GIVEN_SOCKET
	{
		if ( who != INVALID_SOCKET )		
			send ( who, buffer, (int)length, 0 );
	}
}