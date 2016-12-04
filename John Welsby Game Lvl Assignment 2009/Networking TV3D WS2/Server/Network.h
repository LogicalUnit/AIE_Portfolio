#pragma once



class Network
{
public:
	Network		();
	~Network	();

	void	InitializeWinsock	();
	void	TerminateWinsock	();

	NetMessage	*ProcessMessage		( WPARAM wParam, LPARAM lParam );

	void	OpenServerSocket	( HWND wnd, int port );
	void	OpenClientSocket	( HWND wnd, const char* addr, int port );
	void	Disconnect			( );

	enum	SendRule
	{
		EXCEPT_GIVEN_SOCKET		= 0,
		ONLY_GIVEN_SOCKET		= 1
	};

	void	Send				( const char* buffer, size_t length );
	void	Send				( const char* buffer, size_t length, SOCKET who, SendRule rule );

	inline bool IsConnected ()	{ return bConnected; }
	inline bool	IsServer ()		{ return bConnected && bServerSocket; }
	inline bool IsClient ()		{ return bConnected && !bServerSocket; }

	inline	int	GetMaxMessageLength () const { return 512; }
	
	// Error handling
	inline int	GetLastError	() { return WSAGetLastError(); }
	const char*	DescribeError	( int winsock_errorcode );

	enum	WMCode
	{
		WM_NETWORK_MESSAGE	= (WM_USER+1)		
	};

private:
	// Clientlist only applies to servers!
	struct	Client
	{
		SOCKET			clientsocket;
		SOCKADDR_IN		clientaddress;
	};
	std::list<Client>	clientlist;


	HWND		hWnd;
	WSADATA		wsaData;
	bool		bConnected;
	bool		bServerSocket;
	SOCKET		mysocket;
	SOCKADDR_IN	socketaddress;
	char		*pReadBuffer;
};