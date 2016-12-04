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

#include <winsock2.H>
#include <exception>
#include <stdio.h>
#include <list>

// TrueVision 3D includes
#include <CTVActor.h>
#include <CTVAtmosphere.h>
#include <CTVCamera.h>
#include <CTVCollisionResult.h>
#include <CTVEngine.h>
#include <CTVInputEngine.h>
#include <CTVMaterialFactory.h>
#include <CTVMathLibrary.h>
#include <CTVMesh.h>
#include <CTVScene.h>
#include <CTVScreen2DImmediate.h>
#include <CTVScreen2DText.h>
#include <CTVTextureFactory.h>
#include <HelperFunctions.h>
#include <tv_types.h> 


#include "Messages.h"
#include "Network.H"
#include "Resource.h"
#include "GameProcessing.h"
#include "Server.h"

// TODO: reference additional headers your program requires here
