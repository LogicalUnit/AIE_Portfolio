// TV3DCreating.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ProceduralFan.h"

// Global Variables:
HINSTANCE hInst;								// current instance
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

// initialise the global variables for our window
HDC		hDC		= NULL;
HWND	hWnd	= NULL;

CTVMesh				*pFan = NULL;
bool				fanOn = true;
std::vector<int>	Lights;

// we are going to use a vector to store the mesh details
std::vector<ObjectData> Level1List;
std::vector<CTVMesh*> MeshList;

bool		done = false;
bool		clipping = true;
float		dt;


// input and camera variables 
float camPositionX = 0.0;
float camPositionY = 10.0;
float camPositionZ = -100.0;
float camLookAtX = -0.069f;
float camLookAtY = 10.2f;
float camLookAtZ = -99.0f;
float camAngleX = 0.0f;
float camAngleY = 0.0f;
float camWalk = 0.0f;
float camStrafe = 0.0f;
float camUp = 0.0f;
float camSpeed = 1.0f;
float fanSpeed = 0.1f;

// Main entry point to the program
int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// ignore unused command line parameters
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// window message variable
	MSG msg;

	// attempt to create our render window - name, width, height, bit depth
	if( ! CreateTVRenderWindow( "TrueVision3D Tutorial", 800, 600, 16 ) )
	{
		MessageBox( NULL, "Window Initialisation Failed", "Error", MB_OK | MB_ICONINFORMATION );
		return ( 0 );
	}

	// start our engine and load up everything
	InitTV3D( hWnd );

	// Main loop
	while( ! done )
	{
		// Only update the render if we have focus to it.
		if ( GetFocus() == hWnd )
		{
			dt = EngineManager::Instance()->GetEngine()->AccurateTimeElapsed();

			// clears the buffers
			EngineManager::Instance()->GetEngine()->Clear(false);

			Input();

			// sets a flag to say render everything in the scene by default
			EngineManager::Instance()->GetScene()->RenderAll(true);

			DebugData();

			// now we actually render the scene
			EngineManager::Instance()->GetEngine()->RenderToScreen();

			// check for esc press
			//if( pInput->IsKeyPressed( cTV_KEY_ESCAPE ) )
			//	done = true;
		}
		else
		{
			// So we don't process the messages too many times if we are not rendering.
			Sleep( 100 );
		}

		// Process normal window messages
		if ( PeekMessage( &msg, NULL, 0,0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	// Make sure we clean up after ourselves
	CleanTV3D();

	// exit to windows
	return (int) msg.wParam;
}
/***************************************/


/** CleanTV3D **************************/
void CleanTV3D()
{
	// just in case
	if( !Level1List.empty() )
	{
		// delete all
		Level1List.erase( Level1List.begin(), Level1List.end() );		
	}

	if (!MeshList.empty())
	{
		for ( std::vector<CTVMesh*>::iterator i = MeshList.begin(); i != MeshList.end(); i++)
			delete(*i);
	}

	Lights.clear();

	EngineManager::DestroyInstance();
}
/***************************************/

/*** InitTV3D **************************/
bool InitTV3D( HWND hwnd )
{
	EngineManager::Instance()->Init(hwnd);

	// tiny xml - load file
	LoadXML( "tvmexport.xml", Level1List );

	// Load the scene objects 
	if( !LoadScene( Level1List ) )
	{
		return false;
	}

	// can empty the list now - for memory saving
	if( !Level1List.empty() )
	{
		// delete all
		Level1List.erase( Level1List.begin(), Level1List.end() );		
	}
	
	// whew - all is good
	return true;
}
/***************************************/


/***LoadXML*****************************/
// Loads the xml file. 
// This is created specifically for use with
// a vector and specifically for the xml format
/*
<?xml version="1.0" ?> 
  <gamedata>
	  <object>
		  <name>Building1</name> 
		  <pos>23.0652 -34.6581 3.08858</pos> 
		  <orient>0.0 0.0 0.0</orient> 
	  </object> 
	  <object>
		  <name>Building2</name> 
		  <pos>33.0652 -44.6581 3.08858</pos> 
		  <orient>0.0 0.0 0.0</orient> 
	  </object>
  </gamedata>
  */
/***************************************/

void LoadXML(const char* pFilename, std::vector<ObjectData> &list )
{
	CTVEngine *pEngine = EngineManager::Instance()->GetEngine();
	// make sure the list is empty
	if( !list.empty() )
	{
		// delete all
		list.erase( list.begin(), list.end() );
		
	}

	// variables
	ObjectData data;
	char temp[255];
	TiXmlDocument doc(pFilename);

	// make sure the file exists and has a valid header
	if (!doc.LoadFile()) 
	{
		pEngine->AddToLog( "Unable to load xml file" );
		return;
	}

	// some more variables
	TiXmlHandle hDoc(&doc);
	TiXmlElement *pElem, *pSubElem;
	
	// get a pointer to the first element - the root
	pElem = hDoc.FirstChildElement().Element();

	// should always have a valid root but handle gracefully if it doesn't
	if (!pElem) 
		return;

	// print it out if there is one - debugging
	pEngine->AddToLog( "root" );
	pEngine->AddToLog( pElem->Value() );
	pEngine->AddToLog( "\nroot" );
	
	// get the first element	
	pElem = hDoc.FirstChildElement().FirstChildElement().Element();

	// iterate through the parent elements
	for( pElem; pElem; pElem = pElem->NextSiblingElement())
	{	
		// get the first child element
		pSubElem = TiXmlHandle(pElem).FirstChild().Element();
		bool loadData=false;
		
		// iterate through all the child elements
		for( pSubElem; pSubElem; pSubElem = pSubElem->NextSiblingElement())
		{
			// get the information
			const char *pKey = pSubElem->Value();
			const char *pText = pSubElem->GetText();
			

			// make sure it exists
			if (pKey && pText) 
			{
				// print it out - debugging
				pEngine->AddToLog( "key" );
				pEngine->AddToLog( pKey );
				pEngine->AddToLog( "text" );
				pEngine->AddToLog( pText );
				pEngine->AddToLog( "\n" );

				// store the data temporarily
				if( strcmp( pKey, "ObjectName" ) == 0 )
				{
					strcpy_s( data.name, 29, pText );
					loadData = true;
				}
				else if( strcmp( pKey, "Position" ) == 0 )
				{
					strcpy_s( temp, 254, pText );
					data.position = VectorFromString( temp );
				}
				else if( strcmp( pKey, "Rotation" ) == 0 )
				{
					strcpy_s( temp, 254, pText );
					data.orient = VectorFromString( temp );
				}	
				else if( strcmp( pKey, "camspeed") == 0 )
				{
					camSpeed = (float)atof(pText); //custom function to change camera speed via XML
				}
			}
		}
		// add the data to the vector
		if (loadData)
			list.push_back( data );
	}
}
/***************************************/

/*****LoadScene*************************/
// Load scene from the vector. 
// This is created specifically for use with
// a vector holding ObjectData structures.
// ObjectData structures are defined in TV3DCreating.h
/***************************************/
bool LoadScene( std::vector<ObjectData> list )
{
	// make sure it is not empty
	if( !list.empty() )
	{
		// then load the level
		std::vector<ObjectData>::iterator startList = list.begin();
		
		// iterate through the level data
		for( startList; startList != list.end(); startList++ )
		{
			// if we are loading a building - anything with the word building first

				// create the filename
				char buf[30];
				
				strcpy_s( buf, 29, (*startList ).name );
				strcat_s( buf, 29, ".tvm" );
				
				// create the mesh and set the details
				CTVMesh *pMesh = EngineManager::Instance()->GetScene()->CreateMeshBuilder( buf );
				
				if (strcmp(buf, "proceduralfan.tvm") == 0)
				{
					ProceduralFan fan;
					fan.BuildFan(pMesh);
					pFan = pMesh;
				}
				else
				{
					pMesh->LoadTVM( buf );
				}
				
				// truevision has not yet created an option for just sending a vector :(
				pMesh->SetPosition( ( *startList ).position.x, ( *startList ).position.y, ( *startList ).position.z );
				pMesh->SetRotation( ( *startList ).orient.x, ( *startList ).orient.y, ( *startList ).orient.z );
				
				int iLightMat = EngineManager::Instance()->GetMaterialFactory()->CreateLightMaterial( 0.8f, 0.8f, 0.8f, 1.f, 0.4f );		
				pMesh->SetMaterial(iLightMat);
				pMesh->SetLightingMode( cTV_LIGHTING_MANAGED );
				pMesh->SetCollisionEnable(true);
				MeshList.push_back(pMesh);
		}

		SetupLights();
	}
	return true;
}
/***************************************/

/***VectorFromString********************/
// converts a string to a TV3D vector
/***************************************/
cTV_3DVECTOR VectorFromString( char *Line )
{
	cTV_3DVECTOR temp;
	float numList[3] = {0, 0, 0};
	char numPart[15] = "              ";
	char *ptr = Line;
	int i = 0, j = 0;

	while( *ptr != '\0' )
	{		
		if( *ptr == ' ' )
		{
			numList[j] = (float)atof( numPart );
			i = 0;
			memcpy( numPart, "              ", 15 * sizeof(char) );
			++j;
		}
		else
		{
			numPart[i] = *ptr;
			i++;
		}
		ptr++;
	}

	numList[j] = (float)atof( numPart );
	memcpy( numPart, "              ", 15 * sizeof(char) );

	temp.x = numList[0];
	temp.y = numList[1];
	temp.z = numList[2];

	return temp;
}
/***************************************/

/*** The window Event Handler *****************/
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_CLOSE:
		{
			// Make sure we clean up after ourselves
			done = true;
			PostQuitMessage( 0 );
			return(0);
		} 

	case WM_SIZE:								
		{
			return (0);							
		} 

	default: 
		break;
	}

	return ( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
}
/***************************************/

/*******CreateTVRenderWindow************/
bool CreateTVRenderWindow( char * title,  int width, int height, int bits ) 
{
	// get the handle for the window
	hInst = GetModuleHandle( NULL );

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_TEMPLATE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= "TV3DWindow";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	// register the window
	if( ! ( RegisterClassEx( &wcex ) ) )
	{
		MessageBox( NULL, "Class Registration Failed", "ERROR", MB_OK | MB_ICONEXCLAMATION );
		return false;
	}

	// create the window
	if( ! ( hWnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,  "TV3DWindow", title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
		CW_USEDEFAULT,  width,  height,  NULL, NULL, hInst, NULL ) ) )
	{
		MessageBox( NULL, "Window Creation Failed", "ERROR", MB_OK | MB_ICONINFORMATION );
		return false;
	}

	// display the window
	ShowWindow( hWnd, SW_SHOW );
	SetForegroundWindow( hWnd );
	SetFocus( hWnd );

	return true;
}
/***************************************/
void Input()
{
	bool justPressed = false;
	static bool currentDown = false;

	CTVInputEngine *pInput = EngineManager::Instance()->GetInput();
	CTVCamera *pCamera = EngineManager::Instance()->GetCamera();

	cTV_3DVECTOR oldPos = pCamera->GetPosition();


	if (pInput->IsKeyPressed(cTV_KEY_W))
		camWalk = camSpeed;

	if (pInput->IsKeyPressed(cTV_KEY_S))
		camWalk = -camSpeed;

	if (pInput->IsKeyPressed(cTV_KEY_A))
		camStrafe = camSpeed;

	if (pInput->IsKeyPressed(cTV_KEY_D))
		camStrafe = -camSpeed;
	
	if (pInput->IsKeyPressed(cTV_KEY_Q))
		camUp = camSpeed;

	if (pInput->IsKeyPressed(cTV_KEY_Z))
		camUp = -camSpeed;

	if ( pInput->IsKeyPressed(cTV_KEY_E) && !currentDown )
	{
		justPressed = true;
		currentDown = true;
	}

	if ( !pInput->IsKeyPressed(cTV_KEY_E))
		currentDown = false;

	if (justPressed)  //Switch the fan on or off when 'e' is pressed.
		fanOn = !fanOn;

	if (pInput->IsKeyPressed(cTV_KEY_ESCAPE))
		done = true;

	if (pInput->IsKeyPressed(cTV_KEY_N))
		clipping = false;

	if (pInput->IsKeyPressed(cTV_KEY_C))
		clipping = true;



	int tmpMouseX = 0, tmpMouseY = 0;
	pInput->GetMouseState(&tmpMouseX, &tmpMouseY);

	camAngleX = camAngleX - ((float)tmpMouseY / 100.0f);
    camAngleY = camAngleY - ((float)tmpMouseX / 100.0f);

	float fTimeElapsed = dt;

	camPositionX = camPositionX + (cosf(camAngleY) * camWalk * fTimeElapsed) + (cosf(camAngleY + 3.141596f / 2.0f) * camStrafe * fTimeElapsed);
    camPositionZ = camPositionZ + (sinf(camAngleY) * camWalk * fTimeElapsed) + (sinf(camAngleY + 3.141596f / 2.0f) * camStrafe * fTimeElapsed);
	camPositionY = camPositionY + camUp * fTimeElapsed;
   
	camLookAtX = camPositionX + cosf(camAngleY);
    camLookAtY = camPositionY + tanf(camAngleX);
    camLookAtZ = camPositionZ + sinf(camAngleY);


	pCamera->SetPosition(camPositionX, camPositionY, camPositionZ);
	pCamera->SetLookAt(camLookAtX, camLookAtY, camLookAtZ);

	cTV_3DVECTOR newPos = pCamera->GetPosition();
	
	if (clipping) 
	{
		if (EngineManager::Instance()->GetScene()->Collision(&newPos, &oldPos)) //camera collision
		{	
			camPositionX = oldPos.x; 
			camPositionY = oldPos.y; 
			camPositionZ = oldPos.z;
			pCamera->SetPosition(camPositionX, camPositionY, camPositionZ);
		}
	}

	if(fanOn && pFan)
		pFan->RotateY(fanSpeed * fTimeElapsed);

	camStrafe = camWalk = camUp = 0.0f;	
}

/**
	Displays map coordinates and instructions to user.
*/
void DebugData()
{
	CTVScreen2DText *pText = EngineManager::Instance()->GetText();
	
	std::ostringstream output;
	output<<"John Welsby Adv Dip Assigment 1 2009"<<std::endl;
	output<<"Use W A S D to navigate the XZ plane"<<std::endl;
	output<<"Use Q Z to navigate up and down the Y axis"<<std::endl;
	output<<"x: "<<camPositionX<<" y: "<<camPositionY<<" z: "<<camPositionZ<<std::endl;
	output<<"look x: "<<camLookAtX<<" look y: "<<camLookAtY<<" look z: "<<camLookAtZ<<std::endl;
	output<<"Press N to disable collisions; C to enable"<<std::endl;
	output<<"collisions: "<<clipping<<" "<<std::endl;
	output<<"Press E to enable and disable procedural animation"<<std::endl;
	output<<"fan: "<<fanOn;
	pText->Action_BeginText();
	pText->NormalFont_DrawText( output.str().c_str(), 10, 40, cTV_COLORKEY_GREEN );
	pText->Action_EndText();
}

/**
	Simple helper function to create a point light at a specific location
*/
inline void AddPointLight(cTV_3DVECTOR pos)
{
	CTVLightEngine *pLightE = EngineManager::Instance()->GetLightEngine();
	int light = pLightE->CreatePointLight( &pos, 1.f, 1.f, 1.f, 500.f );
	Lights.push_back(light);
}

/**
	This function is used to specify the lighting objects for the level.
	I have decided to use point lights to create a dimly-lit Doom3-esque map.
*/
void SetupLights()
{
	CTVLightEngine *pLightE = EngineManager::Instance()->GetLightEngine();

	//Add additional lights here
	AddPointLight(cTV_3DVECTOR(1236.f, 600.f, 233.f));
	AddPointLight(cTV_3DVECTOR(1264.f, 600.f, -1260.f));
	AddPointLight(cTV_3DVECTOR(145.f, 477.f, -1680.f));
	AddPointLight(cTV_3DVECTOR(-896.f, 291.f, -1480.f));
	AddPointLight(cTV_3DVECTOR(-380.f, 615.f, -125.f));
	AddPointLight(cTV_3DVECTOR(-1224.f, 185.f, 727.f));
	AddPointLight(cTV_3DVECTOR(507.f, 606.f, -414.f));

	for(std::vector<int>::iterator i = Lights.begin(); i != Lights.end(); i++)
		pLightE->EnableLight(*i, true);
}