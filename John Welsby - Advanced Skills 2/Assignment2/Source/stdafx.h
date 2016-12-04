// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <Mmsystem.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

//C++ STD headers
#include <vector>
#include <sstream>
#include <string>

using namespace std;

//XML Includes
#include "TinyXMLHeader.h"


#include <tv_types.h>

inline float magnitude(cTV_3DVECTOR vec)
{
	float mag = pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2);
	return sqrt(mag);
}