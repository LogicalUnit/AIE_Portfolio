/**
	EngineManager.h

	This singleton class encompasses most of the TrueVision 3D functionality.
	It contains accessor methods for the commonly used classes in a TV3D project.
	It is responsible for setup and destruction of the game engine.
*/

#pragma once
#include "stdafx.h"
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

// TrueVision 3D includes



class EngineManager
{
	public:
//****************************************************************************************
		//Singleton stuff
		//Used to get the current instance
		static EngineManager *Instance(void);

		//Used to destroy the current instnace
		static int DestroyInstance(void);
//****************************************************************************************

		bool Init( HWND hwnd, int Width = 1024, int Height = 768, bool Windowed = true);

		CTVScene *GetScene(void)
		{ return pScene; };

		CTVCamera *GetCamera(void)
		{ return pCamera; };

		CTVEngine *GetEngine(void)
		{ return pEngine; };

		CTVInputEngine *GetInput(void)
		{ return pInput; };

		CTVMathLibrary *GetMath(void)
		{  return pMath; };

		CTVTextureFactory *GetTextureFactory(void)
		{ return pTexFactory; };

		CTVScreen2DText *GetText(void)
		{ return pText; };

		CTVLightEngine *GetLightEngine(void)
		{ return pLightEngine; };

		CTVMaterialFactory *GetMaterialFactory(void)
		{ return pMaterialFactory; };


		HWND GetHandle(void)
		{ return m_Hwnd; };

	//Keep the constructors and destructions private of protected
	//Makes sure you can only ever have one instance of the class
	protected:
		EngineManager(void);
		~EngineManager(void);

	private:
		CTVEngine			*pEngine;
		CTVScene			*pScene;
		CTVCamera			*pCamera;
		CTVInputEngine		*pInput;
		CTVTextureFactory	*pTexFactory;
		CTVMathLibrary		*pMath;
		CTVScreen2DText		*pText;
		CTVLightEngine		*pLightEngine;
		CTVMaterialFactory	*pMaterialFactory;


		HWND m_Hwnd;

//****************************************************************************************
		static EngineManager *pInstance;
//****************************************************************************************
};

