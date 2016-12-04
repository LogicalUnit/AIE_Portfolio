/*
	Level.h
	John Welsby 
	2009-05-13

	This header file declares functions and data structures used in main.cpp for constructing the 
	level geometry.
*/

#pragma once
#include <tv_types.h>

struct ObjectData
{
	char name[30];
	cTV_3DVECTOR position,
				 orient;
};

// XML related functions
void LoadXML(const char* pFilename, std::vector<ObjectData> &list );
bool LoadScene( std::vector<ObjectData> list );
cTV_3DVECTOR VectorFromString( char *Line );


void SetupLights();