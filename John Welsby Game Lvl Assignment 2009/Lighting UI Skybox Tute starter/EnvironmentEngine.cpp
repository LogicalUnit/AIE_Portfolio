#include "StdAfx.h"
#include "EnvironmentEngine.h"

EnvironmentEngine::EnvironmentEngine(void)
{
}

EnvironmentEngine::~EnvironmentEngine(void)
{
	if (pLight)
	{
		delete (pLight);
		pLight = NULL;
	}

	if (pAtmosphere)
	{
		delete(pAtmosphere);
		pAtmosphere = NULL;
	}

	if (pMaterial)
	{
		delete (pMaterial);
		pMaterial = NULL;
	}

}

void EnvironmentEngine::Initialise(void)
{
	//Create instances of all the main classes
	pLight = new CTVLightEngine();
	pAtmosphere = new CTVAtmosphere();
	pTexture = EngineManager::Instance()->GetTextureFactory();
	pMaterial = new CTVMaterialFactory();

	// turn off all ambient lighting to start
	pLight->SetGlobalAmbient( 0.f, 0.f, 0.f );

	//Create the 3 lights
	//Create a directional light Direction to shine in: position & Colours: R G B Values 0.0 to 1.0
	LightList[0] = pLight->CreateDirectionalLight( &cTV_3DVECTOR( 1.f, -1.f, 1.f), 0.8f, 0.2f, 0.2f );
	//Create a point light Light Location Colours R G B Radius
	LightList[1] = pLight->CreatePointLight( &cTV_3DVECTOR( 0.f, 15.f, 0.f), 0.2f, 0.8f, 0.2f, 50.f );
	//Create a spot light
	LightList[2] = pLight->CreateSpotLight( &cTV_3DVECTOR( 200.f, 100.f, 200.f), // Light Location
	&cTV_3DVECTOR( 0.f, -1.f, -1.f), // Direction to shine
	0.1f, 0.1f, 0.8f , // Colour
	40.f, 20.f, 10.f ); // Radius Outer, Inner cones

	//Set to Directional light on start
	SwitchLightType(1);
	//set some properties for each of the lights — there are more if you look them up
	/*pLight->SetLightRange( LightList[0],1500 );
	pLight->SetLightProperties(LightList[0], true, true, false);
	pLight->EnableLight( LightList[0], false );
	pLight->SetLightRange( LightList[1], 1500 );
	pLight->SetLightProperties(LightList[1], true, true, false);
	pLight->EnableLight( LightList[1], false );
	pLight->SetLightRange( LightList[2], 1500 );
	pLight->SetLightSpotFalloff( LightList[2], 1 );
	pLight->SetLightProperties(LightList[2], true, true, false);
	pLight->EnableLight( LightList[2], false );
	pLight->SetSpecularLighting( true );*/




	//Load Textures for the sky box
	//Front, Back, Left, Right, Top, Bottom
	SkyBoxList[0] = pTexture->LoadTexture("./SkyBox/front.jpg", "Front" );
	SkyBoxList[1] = pTexture->LoadTexture("./SkyBox/back.jpg", "back" );
	SkyBoxList[2] = pTexture->LoadTexture("./SkyBox/Left.jpg", "Left" );
	SkyBoxList[3] = pTexture->LoadTexture("./SkyBox/Right.jpg", "Right" );
	SkyBoxList[4] = pTexture->LoadTexture("./SkyBox/Top.jpg", "Front" );
	SkyBoxList[5] = pTexture->LoadTexture("./SkyBox/Bottom.jpg", "Bottom" );

	//Set the textures for the skybox
	pAtmosphere->SkyBox_SetTexture( SkyBoxList[0], SkyBoxList[1], SkyBoxList[2],
									SkyBoxList[3], SkyBoxList[4], SkyBoxList[5] );

	//Enable the skybox
	pAtmosphere->SkyBox_Enable(true);

}

void EnvironmentEngine::SwitchLightType( int Type )
{
	//And Within the Function SwitchLightType add the following:
	//Turn all the lights off
	pLight->EnableLight(LightList[0] , false );
	pLight->EnableLight(LightList[1] , false );
	pLight->EnableLight(LightList[2] , false );
	//Turn on the one we are using
	pLight->EnableLight(LightList[Type-1] , true );
}