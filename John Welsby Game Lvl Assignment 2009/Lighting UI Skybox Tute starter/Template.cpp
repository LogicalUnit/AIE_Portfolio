#include "stdafx.h"

// initialise the global variables for our window
HDC			hDC			= NULL;
HWND		hWnd		= NULL;
HINSTANCE	hInstance	= NULL;

// initialise the global variables for TrueVision

extern bool done;

CTVMesh *pGround;
CTVActor *pActor[4];

/*** InitTV3D **************************/
bool InitTV3D( HWND hwnd )
{
	//Init the engine
	EngineManager::Instance()->Init(hwnd);	

	//Init the Enviromentals
	EnvironmentEngine::Instance()->Initialise();

	//Init the interface
	GUIManager::Instance()->Initialise();

	//Create objects for the scene
	pGround = EngineManager::Instance()->GetScene()->CreateMeshBuilder();
	pActor[0] = EngineManager::Instance()->GetScene()->CreateActor();

	//Load the actor up, set up animations etc
	pActor[0]->LoadTVA("Bruce.TVA");
	pActor[0]->SetPosition( -50.f, 0.f, -50.f);
	pActor[0]->SetScale( 0.5f, 0.5f, 0.5f);
	pActor[0]->SetLightingMode(cTV_LIGHTING_MANAGED);
	pActor[0]->SetShadowCast(true, true); //Shadows!
	pActor[0]->SetAnimationByName("idle");
	pActor[0]->PlayAnimation(0.5f);

	pActor[1] = pActor[0]->Duplicate();
	pActor[1]->SetPosition( -50.f, 0.f, 50.f);
	pActor[1]->SetAnimationByName("idle");
	pActor[1]->PlayAnimation(0.5f);

	pActor[2] = pActor[0]->Duplicate();
	pActor[2]->SetPosition( 50.f, 0.f, 50.f);	
	pActor[2]->SetAnimationByName("idle");
	pActor[2]->PlayAnimation(0.5f);

	pActor[3] = pActor[0]->Duplicate();
	pActor[3]->SetPosition( 50.f, 0.f, -50.f);
	pActor[3]->SetAnimationByName("idle");
	pActor[3]->PlayAnimation(0.5f);

	//Load the grass texture
	int iGrass = EngineManager::Instance()->GetTextureFactory()->LoadTexture("grass.jpg", "grass");
	//Create a light material															  Colours Red, Green, Blue, Alpha | Specular
	int iLightMaterial = EnvironmentEngine::Instance()->GetMaterialFactory()->CreateLightMaterial( 0.8f, 0.8f, 0.8f, 1.f, 0.4f);

	// set the material settings more to get even better effets
	EnvironmentEngine::Instance()->GetMaterialFactory()->SetEmissive( iLightMaterial, 0.2f, 0.2f, 0.2f, 1.f );
	EnvironmentEngine::Instance()->GetMaterialFactory()->SetAmbient( iLightMaterial, 0.0f, 0.0f, 0.0f, 1.f );
	EnvironmentEngine::Instance()->GetMaterialFactory()->SetDiffuse( iLightMaterial, 0.5f, 0.5f, 0.5f, 1 );

	//Create a floor for the scene
	pGround->AddFloor( iGrass, -200.f, -200.f, 200.f, 200.f, 0.f, 5.f, 5.f );
	
	//Give it a light material
	pGround->SetMaterial(iLightMaterial);
	//Enable lighting on it
	pGround->SetLightingMode(cTV_LIGHTING_MANAGED);


	return true;
}
/***************************************/

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	if( ! CreateTVRenderWindow( "Lighting, SkyBox, UI, Singleton Tutorial", 1024, 768, 32 ) )
	{
		MessageBox( NULL, "Window Initialisation Failed", "Error", MB_OK | MB_ICONINFORMATION );
		return ( 0 );
	}

	InitTV3D( hWnd );

	// Main loop
	while( ! done )
	{
		// Only update the render if we have focus to it.
		if ( GetFocus() == hWnd )
		{
			if( EngineManager::Instance()->GetInput()->IsKeyPressed( cTV_KEY_ESCAPE ) )
				done = true;
			
			// Get the tickcount so updates are more smooth depending on video card
			float fTickCount = EngineManager::Instance()->GetEngine()->AccurateTimeElapsed();

			//render stuff
			EngineManager::Instance()->GetEngine()->Clear(false);

			// rather than doing render all we are rendering individually in the order we want

			//Render the skybox - its the furthest away
			EnvironmentEngine::Instance()->RenderSkyBox();

			// render the ground next
			pGround->Render();

			//render the characters in a square formation				
			pActor[0]->Render();
			pActor[1]->Render();
			pActor[2]->Render();
			pActor[3]->Render();

			EngineManager::Instance()->GetScene()->FinalizeShadows();
			//Draw and update the GUI
			GUIManager::Instance()->Update();

			EngineManager::Instance()->GetEngine()->RenderToScreen();		
				
		}
		else
		{
			// So we dont process the messages too many times if we are not rendering.
			Sleep( 100 );
		}

		// Process normal messages
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	// Make sure we clean up after ourselves
	CleanTV3D();

	return (int) msg.wParam;
}


/** CleanTV3D **************************/
void CleanTV3D()
{
	EngineManager::DestroyInstance();
}


/*** The Event Handler *****************/
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
		case WM_CLOSE:
		{
			// Make sure we clean up after ourselves
			done = true;
			CleanTV3D();
			PostQuitMessage( 0 );
			return(0);
		} break;

		case WM_SIZE:								
		{
			return 0;							
		} break;

		default: 
			break;
	}

	return ( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
}

bool CreateTVRenderWindow( char *title, int width, int height, int bits )
{
	// variable for the windows class
	WNDCLASSEX wc;

	// define the window here
	hInstance = GetModuleHandle( NULL );

	wc.cbSize			= sizeof( WNDCLASSEX );
	wc.style			= CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon( NULL, IDI_WINLOGO );
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "TV3DWindow";
	wc.hIconSm			= LoadIcon( NULL, IDI_APPLICATION );

	if( ! ( RegisterClassEx( &wc ) ) )
	{
		MessageBox( NULL, "Class Registration Failed", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;
	}

	if( ! ( hWnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, "TV3DWindow", title,	WS_OVERLAPPEDWINDOW,
		                           CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL ) ) )
	{
		MessageBox( NULL, "Window Creation Failed", "ERROR", MB_OK | MB_ICONINFORMATION );
		return false;
	}

	ShowWindow( hWnd, SW_SHOW );
	SetForegroundWindow( hWnd );
	SetFocus( hWnd );

	return true;
}

