#pragma once
#include "zsingleton.h"



class GUIManager :
	public zSingleton<GUIManager>
{
	friend zSingleton;
protected:
	GUIManager(void);
	~GUIManager(void);

public:
	void Initialise(void);

	cTV_2DVECTOR ConvertMouseCoordsToTextureCoords();

	void Update(void);

	int GetSelection(void);

private:
	CTVScreen2DImmediate *p2D;
	CTVScreen2DText		 *p2DText;
	int					 iFont;

	int					 iSelectionTextures[3];
	int					 iGUITexture;
	int					 iMaskTexture;
};
