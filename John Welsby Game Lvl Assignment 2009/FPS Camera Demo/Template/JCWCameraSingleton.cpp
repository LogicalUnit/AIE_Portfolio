#include "StdAfx.h"
#include "JCWCameraSingleton.h"

JCWCameraSingleton *JCWCameraSingleton::pSingleton;



JCWCameraSingleton::JCWCameraSingleton(void) : pCamera(NULL)
{
	pCamera = new CTVCamera();
}

JCWCameraSingleton::~JCWCameraSingleton(void)
{
	if (pCamera)
	{
		delete (pCamera);
		pCamera = NULL;
	}

	pSingleton = NULL;
}

JCWCameraSingleton *JCWCameraSingleton::GetInstance()
{
	if(pSingleton == NULL)
		pSingleton = new JCWCameraSingleton();

	return pSingleton;
}

void JCWCameraSingleton::DeleteInstance()
{
	if ( pSingleton )
		delete ( pSingleton );

	pSingleton = NULL;
}

void JCWCameraSingleton::SetViewFrustum(float fFOVAngleDegrees, float fFarPlane)
{
	pCamera->SetViewFrustum(fFOVAngleDegrees, fFarPlane);
}

void JCWCameraSingleton::SetLookAt(float fX, float fY, float fZ)
{
	pCamera->SetLookAt(fX, fY, fZ);
}

void JCWCameraSingleton::SetPosition(float fX, float fY, float fZ)
{
	pCamera->SetPosition(fX, fY, fZ);
}