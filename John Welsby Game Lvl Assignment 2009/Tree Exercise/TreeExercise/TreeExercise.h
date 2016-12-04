#pragma once

#include "resource.h"
bool CreateTVRenderWindow( char*, int, int, int );
bool InitTV3D( HWND hwnd );
void CleanTV3D();
void SetupLights();
void Input();
void DebugData();