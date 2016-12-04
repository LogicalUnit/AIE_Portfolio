#include "StdAfx.h"
#include "GUIManager.h"

GUIManager::GUIManager(void)
{
}

GUIManager::~GUIManager(void)
{
	if ( p2D )
	{
		delete ( p2D );
		p2D = NULL;
	}

	if ( p2DText )
	{
		delete ( p2DText );
		p2DText = NULL;
	}
}

void GUIManager::Initialise()
{
	//Create instances of 2D Drawing classes
	p2D = new CTVScreen2DImmediate();
	p2DText = new CTVScreen2DText();

	//Create a font for use if you ever decide to use it
	iFont = p2DText->NormalFont_Create("Courier", "Courier", 20, false, false, false );
	
	//Load the Various textures
	iGUITexture = EngineManager::Instance()->GetTextureFactory()->LoadTexture("./UI/GUI_Display.png", "MainGUI", -1, -1, cTV_COLORKEY_USE_ALPHA_CHANNEL);
	iMaskTexture = EngineManager::Instance()->GetTextureFactory()->LoadTexture("./UI/buttons.png");
	iSelectionTextures[0] = EngineManager::Instance()->GetTextureFactory()->LoadTexture("./UI/GUI_Directional.png", "DirHighLight", -1, -1, cTV_COLORKEY_USE_ALPHA_CHANNEL);
	iSelectionTextures[1] = EngineManager::Instance()->GetTextureFactory()->LoadTexture("./UI/GUI_Point.png", "PointHighLight", -1, -1, cTV_COLORKEY_USE_ALPHA_CHANNEL);
	iSelectionTextures[2] = EngineManager::Instance()->GetTextureFactory()->LoadTexture("./UI/GUI_Spot.png", "SpotHighLight", -1, -1, cTV_COLORKEY_USE_ALPHA_CHANNEL);


}

void GUIManager::Update(void)
{
	//The Window is 1024x768 BUT the ViewPort is smaller, as the window has a border and title bar
	float ViewPortWidth = (float)EngineManager::Instance()->GetEngine()->GetViewport()->GetWidth();
	float ViewPortHeight = (float)EngineManager::Instance()->GetEngine()->GetViewport()->GetHeight();
	
	//Begin the 2d draw sequence
	p2D->Action_Begin2D();
	
	//Get Current Selection
	int iSelection = GetSelection();
	
	//Draw Highlighted Texture if there is one selected
	if ( iSelection )
	{
		p2D->Draw_Texture( iSelectionTextures[iSelection-1], 0.f, 0.f, ViewPortWidth, ViewPortHeight );
		//Update the LightEngine to the new selection
		EnvironmentEngine::Instance()->SwitchLightType( iSelection );
	}
	else //Draw the normal unhighlighted UI
		p2D->Draw_Texture( iGUITexture, 0.f, 0.f, ViewPortWidth, ViewPortHeight );
	
	//If T is pressed, display the mask texture
	if( EngineManager::Instance()->GetInput()->IsKeyPressed( cTV_KEY_T ) )
		p2D->Draw_Texture( iMaskTexture, 0.f, 0.f, ViewPortWidth, ViewPortHeight );
	
	p2D->Action_End2D();
}

cTV_2DVECTOR GUIManager::ConvertMouseCoordsToTextureCoords()
{
	//The Window is 1024x768 BUT the ViewPort is smaller, as the window has a border and title bar
	float ViewPortWidth = (float)EngineManager::Instance()->GetEngine()->GetViewport()->GetWidth();
	float ViewPortHeight = (float)EngineManager::Instance()->GetEngine()->GetViewport()->GetHeight();

	//Some Mouse Stuff
	int X, Y;
	bool Button1 = false;

	//Get Current Mouse Coordinates
	EngineManager::Instance()->GetInput()->GetAbsMouseState( &X, &Y, &Button1 );
	
	//Now even though the texture is 1024x768 DirectX wants texture sizes in powers of 2
	//So the real texture size in memory is 1024x1024
	//So with the mouse coords, known viewport size and texture size
	//we need to calculate where it is on the texture
	cTV_2DVECTOR FinalMouseVec(0.f, 0.f);

	float ScaleX = 1024.f / ViewPortWidth;
	float ScaleY = 1024.f / ViewPortHeight;
	
	FinalMouseVec.x = (float)X * ScaleX;
	FinalMouseVec.y = (float)Y * ScaleY;

	//Finally do some checks to see if its out of bounds
	if ( FinalMouseVec.x < 0.f )
		FinalMouseVec.x = 0.f;

	if ( FinalMouseVec.y < 0.f )
		FinalMouseVec.y = 0.f;

	if ( FinalMouseVec.x > 1023.f )
		FinalMouseVec.x = 1023.f;

	if ( FinalMouseVec.y > 1023.f )
		FinalMouseVec.y = 1023.f;

	return FinalMouseVec;	
}

int GUIManager::GetSelection()
{
	cTV_2DVECTOR MouseCoord = ConvertMouseCoordsToTextureCoords();
	//Get the colour of the pixel the mouse is currently over
	int iMaskPixelColour = EngineManager::Instance()->GetTextureFactory()->GetPixel( iMaskTexture,
	(int)MouseCoord.x, (int)MouseCoord.y );
	int iSelection = 0;
	switch (iMaskPixelColour)
	{
		case cTV_COLORKEY_BLACK:
		//Check if mouse is near the top of screen
		if ( MouseCoord.y < 100.f )
		{
			//Check which box its in
			if ( MouseCoord.x < 341.f )
			{
			iSelection = 1;
			return iSelection;
		}
		if ( MouseCoord.x < 682.f )
		{
			iSelection = 2;
			return iSelection;
		}
		iSelection = 3;
		return iSelection;
	}
		break;
		//Direction button is red
		case cTV_COLORKEY_RED:
		iSelection = 1;
		return iSelection;
		//Point button is Green
		case cTV_COLORKEY_GREEN:
		iSelection = 2;
		return iSelection;
		//Spot Button is Blue
		case cTV_COLORKEY_BLUE:
		iSelection = 3;
		return iSelection;
	};
	return iSelection;
}