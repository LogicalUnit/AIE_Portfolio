#pragma once

// initialise the global variables for our window
static HDC			hDC			= NULL;
static HWND			hWnd		= NULL;
static HINSTANCE	hInstance	= NULL;

/////////////////////////////////////////////
static HWND		g_hWndEditbox	= 0;
static HWND		g_hWndInputbox	= 0;

// Local attributes
static int	iDlgResult			= 0;
static int	iHost				= 0;
static char	strIPAddress[128]	= "127.0.0.1";
static char	strUsername[128]	= "Server";
static int	iPort				= 45000;

// Local winsock-specific variables
static bool		bWinsockStarted = false;
static WSADATA	wsaData;
static Network	g_Network;

// initialise the global variables for TrueVision
static CTVEngine		*pEngine;

static bool				bIsDone = false;

bool CreateServerWindow( char*, int, int );
bool InitServer( HWND hwnd );
void CleanServer();
void PrintToOutput ( const char* str );

LRESULT CALLBACK WndProc			( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
