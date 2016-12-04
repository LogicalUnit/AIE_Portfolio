#include "StdAfx.h"
#include "Engine.h"

extern bool done = false;

//you have to intialise static variables
EngineManager *EngineManager::pInstance = 0;

//Static function to get an the instance of the class
//and if the isnt one than create one
EngineManager *EngineManager::Instance()
{
	if (!pInstance)
	{
		pInstance = new EngineManager();
	}
	return pInstance;
}

//Destroy the current instance if it exists
//VS.2005 changed static functions, so they must return an int
int EngineManager::DestroyInstance()
{
	if (pInstance)
	{
		delete (pInstance);
		pInstance = 0;
		return true;
	}
	return false;
}

EngineManager::EngineManager(void)
{
	pCamera = NULL;
	pScene = NULL;
	pEngine = NULL;
}

EngineManager::~EngineManager(void)
{
	if ( pTexFactory )
	{
		delete (pTexFactory);
		pTexFactory = NULL;
	}

	if ( pInput )
	{
		delete(pInput);
		pInput = NULL;
	}

	if( pCamera )
	{
		delete( pCamera );
		pCamera = NULL;
	}

	if( pScene )
	{
		pScene->DestroyAllMeshes();
		delete( pScene );
		pScene = NULL;
	}

	if( pEngine )
	{
		delete( pEngine );
		pEngine = NULL;
	}
}

bool EngineManager::Init( HWND hwnd, int Width, int Height, bool Windowed )
{
	m_Hwnd = hwnd;

	//Do some randomizing
	srand((unsigned int)time(NULL));
	pEngine = new CTVEngine();

	//Set debug, and default search directory for files
	pEngine->SetSearchDirectory( "./" );
	pEngine->SetDebugFile( "Debug.txt" );
	pEngine->SetDebugMode( true, false );

	// Init the TV renderer and pass it the handle to the window
	if (Windowed)
	{
		if( !pEngine->Init3DWindowed( hwnd, true ) )
			return false;
	}
	else
		if( !pEngine->Init3DFullscreen( Width, Height, 32, true, false, cTV_DEPTHBUFFER_BESTBUFFER, 1.0, hwnd ) )
			return false;


	// set misc values for the TV engine
	pEngine->GetViewport()->SetAutoResize( true );
	pEngine->DisplayFPS( true );
	pEngine->SetAngleSystem( cTV_ANGLE_DEGREE );

	//Create a new scene, and store it
	pScene = new CTVScene();

	// In order to use mouse pick we need to have a cursor enabled in the scene
	pScene->SetCursor( true, 0, 0, 0, 0.f, 0.f );
	
	// Create the camera system with default parametres
	pCamera = new CTVCamera();
	pCamera->SetViewFrustum(60.f, 10000.f, 1.f);
	
	//Set position and lookat
	pCamera->SetPosition( 0.f, 200.f, -300.f );
	pCamera->SetLookAt( 0.f, 0.f, 0.f);

	//Create the initialize the input engine
	pInput = new CTVInputEngine();
	pInput->Initialize(true,true);

	//Create the texture factory
	pTexFactory = new CTVTextureFactory();

	return true;
	
}

