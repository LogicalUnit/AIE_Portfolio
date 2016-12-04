#include "StdAfx.h"
#include "JCWInputSingleton.h"

JCWInputSingleton *JCWInputSingleton::pSingleton;

JCWInputSingleton::JCWInputSingleton(void) : pInput(NULL)
{
	pInput = new CTVInputEngine();
	pInput->Initialize( true, true );
}

JCWInputSingleton::~JCWInputSingleton(void)
{
	if( pInput )
	{
		delete( pInput );
		pInput = NULL;
	}

	pSingleton = NULL;
}

JCWInputSingleton *JCWInputSingleton::GetInstance()
{
	if( pSingleton == NULL )
		pSingleton = new JCWInputSingleton();

	return pSingleton;
}

void JCWInputSingleton::DeleteInstance()
{
	if ( pSingleton )
		delete ( pSingleton );

	pSingleton = NULL;
}

bool JCWInputSingleton::IsKeyPressed(cCONST_TV_KEY key)
{
	if ( pInput == NULL )
		return false;

	return pInput->IsKeyPressed(key);
}

int JCWInputSingleton::GetMouseX()
{
	if( pInput == NULL )
		return 0;

	int posX = 0, posY = 0;
	pInput->GetMousePosition(&posX, &posY);
	return posX;
}

int JCWInputSingleton::GetMouseY()
{
	if ( pInput == NULL )
		return 0;

	int posX = 0, posY = 0;
	pInput->GetMousePosition(&posX, &posY);
	return posY;
}

void JCWInputSingleton::GetMouseState(int *retRelativeX, int *retRelativeY, bool *retButton1, bool *retButton2, bool *retButton3, bool *retButton4, int *retRoll)
{
	pInput->GetMouseState(retRelativeX, retRelativeY, retButton1, retButton2, retButton3, retButton4, retRoll);
}


/*
void Input()
{
    Check if we pressed the UP arrow key, if so, then we are
    walking forward.
    if(pInput->IsKeyPressed(cTV_KEY_UP))
        camWalk = 1;

    If we are not walking forward, maybe we are walking backward
    by using the DOWN arrow? If so, set walk speed to negative.
    if(pInput->IsKeyPressed(cTV_KEY_DOWN))
        camWalk = -1;

    Check if we pressed the LEFT arrow key, if so, then strafe
    on the left.
    if(pInput->IsKeyPressed(cTV_KEY_LEFT))
        camStrafe = 1;

    If we are not strafing left, maybe we want to strafe to the
    right, using the RIGHT arrow? If so, set strafe to negative.
    if(pInput->IsKeyPressed(cTV_KEY_RIGHT))
        camStrafe = -1;

    Now, for the mouse input...
    int tmpMouseX = 0, tmpMouseY = 0;
    bool tmpMouseB1 = 0, tmpMouseB2 = 0, tmpMouseB3 = 0, tmpMouseB4 = 0;
    int tmpMouseScrollOld = 0, tmpMouseScrollNew = 0;

    We pass the actual value of the mouse scroller to the variable
    that holds the old mouse scroller value.
    tmpMouseScrollOld = tmpMouseScrollNew;

    By using GetMouseState, we get the movement of the mouse
    with the speed of the movement. The fastest the mouse movement
    will be, the higher will be the return.
    pInput->GetMouseState(&tmpMouseX, &tmpMouseY, &tmpMouseB1, &tmpMouseB2, &tmpMouseB3, &tmpMouseB4, &tmpMouseScrollNew);

    From the mouse return values, we update the camera angles
    by adding or substracting the mouse return value.
    camAngleX = camAngleX - ((float)tmpMouseY / 100.0f);
    camAngleY = camAngleY - ((float)tmpMouseX / 100.0f);

    We will add a simple check, so we can't look up at more
    than 80 degrees nor down than -80 degrees.
    if(camAngleX > 1.3f) 
		camAngleX = 1.3f;

    if(camAngleX < -1.3f) 
		camAngleX = -1.3f;

    Okay, now for the smothing of the movement... We checked
    above if we were pressing a key. If so, then we updated the
    movement variable to 1 (positive or negative). Here, we
    lower this value until it get to 0. This method give us a
    smoother camera movement. We start by updating the forward
    and backward (walk) movement.

    if(camWalk > 0)
	{
        camWalk = camWalk - 0.05f;
        if(camWalk < 0) camWalk = 0;
	}
    if(camWalk < 0)
	{
        camWalk = camWalk + 0.05f;
        if(camWalk > 0) camWalk = 0;
	}

    Now, we update the left and right (strafe) movement.
    if(camStrafe > 0)
	{
        camStrafe = camStrafe - 0.05f;
        if(camStrafe < 0) camStrafe = 0;
	}
    if(camStrafe < 0)
	{
        camStrafe = camStrafe + 0.05f;
        if(camStrafe > 0) camStrafe = 0;
	}
	
	float fTimeElapsed = dt;
   
	Update the vectors using the angles and positions.
    camPositionX = camPositionX + (cosf(camAngleY) * camWalk * fTimeElapsed) + (cosf(camAngleY + 3.141596f / 2.0f) * camStrafe * fTimeElapsed);
    camPositionZ = camPositionZ + (sinf(camAngleY) * camWalk * fTimeElapsed) + (sinf(camAngleY + 3.141596f / 2.0f) * camStrafe * fTimeElapsed);
   
	camLookAtX = camPositionX + cosf(camAngleY);
    camLookAtY = camPositionY + tanf(camAngleX);
    camLookAtZ = camPositionZ + sinf(camAngleY);

    With the new values of the camera vectors (position and
    look at), we update the scene's camera.
    pScene->SetCamera(camPositionX, camPositionY, camPositionZ, camLookAtX, camLookAtY, camLookAtZ);

	if(pInput->IsKeyPressed(cTV_KEY_ESCAPE))		//Check if ESCAPE has been pressed.
	{		
		done = true;
	}
}
*/