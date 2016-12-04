#pragma once

class JCWCameraSingleton
{
private:
	JCWCameraSingleton(void);
	~JCWCameraSingleton(void);

	static JCWCameraSingleton	*pSingleton;
	CTVCamera					*pCamera;

public:
	

	static JCWCameraSingleton	*GetInstance();
	static void					DeleteInstance();

	void						SetViewFrustum(float fFOVAngleDegrees,float fFarPlane);
	void						SetPosition(float fX, float fY, float fZ);
	void						SetLookAt( float fX, float fY, float fZ );
};
