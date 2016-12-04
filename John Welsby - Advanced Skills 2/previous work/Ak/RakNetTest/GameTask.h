#pragma once

#include "Network.h"

enum STATE
{
	INIT,
	RUNNING
};

class GameTask
{
public:
	GameTask( CTVEngine *pEngine );
	~GameTask(void);
	bool Update( float dt );


private:
	CTVEngine		*pEngine;
	CTVScene		*pScene;
	CTVCamera		*pCamera;
	CTVInputEngine	*pInput;
	CTVMesh			*pBox;
	CTVMathLibrary	*pMath;
	CTVScreen2DText *pText;

	char			screenMessage[128];
	char			packetMessage[128];
	
	STATE			gameState;

	Network			network;


};
