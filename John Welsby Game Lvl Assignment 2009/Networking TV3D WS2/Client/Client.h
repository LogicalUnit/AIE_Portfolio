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
static char	strUsername[128]	= "Lea";
static int	iPort				= 45000;

// Local winsock-specific variables
static bool			bWinsockStarted	= false;
static WSADATA		wsaData;
static Network		g_Network;

static GameTask		*pGameTask;

static CTVEngine	*pEngine;
static bool			bIsDone;


bool CreateTVRenderWindow( char*, int, int, int );
bool InitTV3D( HWND hwnd );
void CleanTV3D();

void PrintToOutput ( const char* str );


LRESULT CALLBACK WndProc			( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
LRESULT CALLBACK NetworkOptProc		( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
