#pragma once

#include "resource.h"

// initialise the global variables for our window
static HDC			hDC			= NULL;
static HWND			hWnd		= NULL;
static HINSTANCE	hInstance	= NULL;

/////////////////////////////////////////////
// Local attributes
static CTVEngine	*pEngine;
static bool			bIsDone;

static GameTask		*pGameTask;


bool CreateTVRenderWindow( char*, int, int, int );
bool InitTV3D(  );
void CleanTV3D();

void PrintToOutput ( const char* str );


LRESULT CALLBACK WndProc			( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );