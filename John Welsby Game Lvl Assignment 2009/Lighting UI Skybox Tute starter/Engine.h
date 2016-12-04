#pragma once
#include "stdafx.h"

#define V2F(A) A.x, A.y, A.z


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

		CTVTextureFactory *GetTextureFactory(void)
		{ return pTexFactory; };

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

		bool			done;

		HWND m_Hwnd;

//****************************************************************************************
		static EngineManager *pInstance;
//****************************************************************************************
};

