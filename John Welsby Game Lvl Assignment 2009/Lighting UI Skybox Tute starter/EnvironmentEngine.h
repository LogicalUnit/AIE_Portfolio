#pragma once
#include "zsingleton.h"

class EnvironmentEngine :
	public zSingleton<EnvironmentEngine>
{
	//Give zSingleton access to our constructors
	friend zSingleton;

protected:
	EnvironmentEngine(void);
	~EnvironmentEngine(void);

public:
	void Initialise(void);

	void SwitchLightType(int Type);

	void RenderSkyBox(void)
	{ pAtmosphere->Atmosphere_Render(); };

	CTVLightEngine *GetLightEngine(void)
	{ return pLight; };

	CTVMaterialFactory *GetMaterialFactory(void)
	{ return pMaterial; };
	
private:
	CTVLightEngine *pLight;
	CTVAtmosphere  *pAtmosphere;
	CTVTextureFactory *pTexture;
	CTVMaterialFactory *pMaterial;
	//Main light list
	int   LightList[3];
	//Skybox texture list
	int   SkyBoxList[6];

};
