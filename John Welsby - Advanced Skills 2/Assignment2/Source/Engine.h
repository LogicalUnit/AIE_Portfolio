/**
	EngineManager.h

	This singleton class encompasses most of the TrueVision 3D functionality.
	It contains accessor methods for the commonly used classes in a TV3D project.
	It is responsible for setup and destruction of the game engine.
*/

#pragma once
//#include "stdafx.h"
#include "TrueVision3DHeader.h"

class Network;
class UserOutput;
class KeyboardIO;
class Actor;



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

		Network *GetNetwork(void)
		{ return pNet; };

		UserOutput *GetUserOutput(void)
		{ return pUserOut; };

		KeyboardIO	*GetKeyboardIO(void)
		{ return pKeyboardIO; };

		Actor *GetPlayerCharacter(void)
		{ return pPlayerCharacter; };

		void SetPlayerName(std::string name) {this->playerName = name;};
		std::string	GetPlayerName() { return playerName;};

		void NetworkUpdate();


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
		Network				*pNet;
		UserOutput			*pUserOut;
		KeyboardIO			*pKeyboardIO;
		Actor				*pPlayerCharacter;

		std::string			playerName;



		HWND m_Hwnd;

//****************************************************************************************
		static EngineManager *pInstance;
//****************************************************************************************
};

