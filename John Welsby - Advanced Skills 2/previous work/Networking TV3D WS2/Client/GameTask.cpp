#include "StdAfx.h"


GameTask::GameTask( CTVEngine *pEngine, Network network )
{
	thisnetwork = network;
	this->pEngine = pEngine;

	pInput = new CTVInputEngine();
	pInput->Initialize( true, true );

	fCurrentAngleHorizontal = 60.f;
	fCurrentAngleVertical = 60.f; 

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
	bool m_pressed = false;
	static bool m_down = false;
	bool t_pressed = false;
	static bool t_down = false;

	// Pan the camera left
	if( pInput->IsKeyPressed( cTV_KEY_A ) )
		//pCamera->MoveRelative( 0.f, 0.f, -0.05f * dt, true );
		pBox->MoveRelative(0.0f, 0.0f, -0.05f *dt);

	// Pan the camera right
	if( pInput->IsKeyPressed( cTV_KEY_D ) )
		//pCamera->MoveRelative( 0.f, 0.f, 0.05f * dt, true );
		pBox->MoveRelative(0.0f, 0.0f, 0.05f * dt);

	// Move the camera forward
	if( pInput->IsKeyPressed( cTV_KEY_W ) )
		//pCamera->MoveRelative( 0.05f * dt, 0.f, 0.f, true );
		pBox->MoveRelative(0.05f*dt, 0.0f, 0.0f);

	// Move the camera backwards
	if( pInput->IsKeyPressed( cTV_KEY_S ) )
		//pCamera->MoveRelative( -0.05f * dt, 0.f, 0.f, true );
		pBox->MoveRelative(-0.05f * dt, 0.0f, 0.0f);

	// Send a move message to server
	if( pInput->IsKeyPressed( cTV_KEY_M ) && !m_down )
	{
		m_pressed = true;
		m_down = true;
	}

	if( !pInput->IsKeyPressed( cTV_KEY_M ) )
		m_down = false;

	if (m_pressed)
	{
		cTV_3DVECTOR vec = pBox->GetPosition();
		PositionMessage msg;
		msg.facing = 2.4f;
		msg.posx = vec.x;
		msg.posy = vec.y;
		msg.posz = vec.z;
		msg.Uid = 2;

		thisnetwork.Send( (char *)&msg, sizeof( PositionMessage ) );
	}

	if( pInput->IsKeyPressed( cTV_KEY_T ) && !t_down )
	{
		t_pressed = true;
		t_down = true;
	}

	if( !pInput->IsKeyPressed( cTV_KEY_T ) )
		t_down = false;

	if (t_pressed)
	{
		TauntMessage msg;
		char* taunt = "John Rules!";
		strcpy_s(msg.message, sizeof(msg.message), taunt);
		thisnetwork.Send( (char*)&msg, sizeof( TauntMessage ) );
	}


	// Clear the screen buffer
	pEngine->Clear(false);	
	

	// pScene will render any objected created through pScene, including the box in this example
	pScene->RenderAll(true);

	BufferedOutput::Instance()->PrintText();

	// Render the final buffer to screen
	pEngine->RenderToScreen();

	

	// Set the flag to true if we have pressed the excape key
	if( pInput->IsKeyPressed( cTV_KEY_ESCAPE ) )
		return false;

	return true;
}

void GameTask::PositionUpdate( PositionMessage *posMessage )
{

	pBox->SetPosition(posMessage->posx, posMessage->posy, posMessage->posz);
	//get the id of the object to be updated
	//set its position and facing

}