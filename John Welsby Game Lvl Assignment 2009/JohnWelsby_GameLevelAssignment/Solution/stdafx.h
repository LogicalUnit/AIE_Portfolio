// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <vector>
#include <sstream>

using namespace std;


// TrueVision 3D includes
#include <CTVActor.h>
#include <CTVAtmosphere.h>
#include <CTVCamera.h>
#include <CTVEngine.h>
#include <CTVInputEngine.h>
#include <CTVLightEngine.h>
#include <CTVMaterialFactory.h>
#include <CTVMathLibrary.h>
#include <CTVMesh.h>
#include <CTVScene.h>
#include <CTVScreen2DImmediate.h>
#include <CTVScreen2DText.h>
#include <CTVTextureFactory.h>
#include <tv_types.h>


//Types

// user defined includes
#include "Engine.h"
#include "Template.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "TV3DCreating.h"

