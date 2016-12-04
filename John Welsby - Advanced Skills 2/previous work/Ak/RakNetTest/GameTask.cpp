#include "StdAfx.h"


GameTask::GameTask( CTVEngine *pEngine )
{
	this->pEngine = pEngine;

	// set the state to init - we haven decided if we are client or server yet
	gameState = INIT;

	pInput = new CTVInputEngine();
	pInput->Initialize( true, true );

	pScene = new CTVScene();
	
	// Create the camera system with default parametres
	pCamera = new CTVCamera();
	pCamera->SetViewFrustum( 60.f, 1000.f, 1.f ); // 60 degree view fustrum is the default
	pCamera->SetPosition( 100.f, 100.f, 100.f );
	pCamera->SetLookAt( 0.f, 0.f, 0.f );

	// Create a simple box so we have something for the camera to look at
	pBox = pScene->CreateMeshBuilder( "Box" );
	pBox->CreateBox( 64.f, 64.f, 64.f, false );
	pBox->SetPosition( 0.f, 0.f, 0.f );
	pBox->SetColor( cTV_COLORKEY_RED );
	pBox->SetLightingMode( cTV_LIGHTING_NORMAL );

	// Create the Math Library
	pMath = new CTVMathLibrary();

	// create on screen text
	pText = new CTVScreen2DText ();
	sprintf_s(screenMessage, 128, "Press (C) for client or (S) for  server\n");
	
	memset(packetMessage, 0, 128);
}

GameTask::~GameTask(void)
{
	if( pCamera )
	{
		delete( pCamera );
		pCamera = NULL;
	}

	if( pInput )
	{
		delete( pInput );
		pInput = NULL;
	}

	if( pScene )
	{
		pScene->DestroyAllMeshes();
		delete( pScene );
		pScene = NULL;
	}
}

bool GameTask::Update( float dt )
{
	// we need to know if we are server or client yet
	if( gameState == INIT )
	{
		// Choose Client
		if( pInput->IsKeyPressed( cTV_KEY_C ) )
		{			
			gameState = RUNNING;
			network.StartClient();
		}

		// Choose server
		if( pInput->IsKeyPressed( cTV_KEY_S ) )
		{			
			gameState = RUNNING;
			network.StartServer();
		}
	}
	else // we know what we are
	{
		// Pan the camera left
		if( pInput->IsKeyPressed( cTV_KEY_A ) )
			pCamera->MoveRelative( 0.f, 0.f, -0.05f * dt, true );

		// Pan the camera right
		if( pInput->IsKeyPressed( cTV_KEY_D ) )
			pCamera->MoveRelative( 0.f, 0.f, 0.05f * dt, true );

		// Move the camera forward
		if( pInput->IsKeyPressed( cTV_KEY_W ) )
			pCamera->MoveRelative( 0.05f * dt, 0.f, 0.f, true );

		// Move the camera backwards
		if( pInput->IsKeyPressed( cTV_KEY_S ) )
			pCamera->MoveRelative( -0.05f * dt, 0.f, 0.f, true );

		// Send a move message to server
		if( pInput->IsKeyPressed( cTV_KEY_M ) )
		{
			ChatMessage msg;
			network.SendChatMessage(&msg);
		}

		strcpy_s (packetMessage, 128, network.Update());
	}

	// Clear the screen buffer
	pEngine->Clear(false);

	// pScene will render any objected created through pScene, including the box in this example
	pScene->RenderAll(true);

	// render 2d text
	pText->Action_BeginText();
	pText->NormalFont_DrawText (screenMessage, 10, 30, cTV_COLORKEY_GREEN );
	pText->NormalFont_DrawText (packetMessage, 10, 60, cTV_COLORKEY_GREEN );
	pText->Action_EndText();


	// Render the final buffer to screen
	pEngine->RenderToScreen();

	// Set the flag to true if we have pressed the excape key
	if( pInput->IsKeyPressed( cTV_KEY_ESCAPE ) )
		return false;

	return true;
}

