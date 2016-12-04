/***************************************/
// Author: Lea Bartlett
/***************************************/

#pragma once

#include "resource.h"

/***************************************/
// structure to hold object data for loading
struct ObjectData
{
	char name[30];
	cTV_3DVECTOR position,
				 orient;
};

/***************************************/
// truevision engine control frunctions
bool CreateTVRenderWindow( char*, int, int, int );
bool InitTV3D( HWND hwnd );
void CleanTV3D();
void Input();	//Calculate user input in each iteration of the main loop
void DebugData(); //Display coordinates and instructions

// XML related functions
void LoadXML(const char* pFilename, std::vector<ObjectData> &list );
bool LoadScene( std::vector<ObjectData> list );

cTV_3DVECTOR VectorFromString( char *Line );
bool LoadScene( std::vector<ObjectData> list );

/***************************************/
