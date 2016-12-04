#pragma once

class GameTask
{
public:
	GameTask( CTVEngine *pEngine, Network network );
	~GameTask(void);
	bool Update( float dt );

	void PositionUpdate( PositionMessage *posMessage );

private:
	Network			thisnetwork;

	CTVEngine		*pEngine;
	CTVScene		*pScene;
	CTVCamera		*pCamera;
	CTVInputEngine	*pInput;
	CTVMesh			*pBox;
	CTVMathLibrary	*pMath;
	
	float			fCurrentAngleHorizontal, fCurrentAngleVertical; 

};
