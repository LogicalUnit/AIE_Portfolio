#include "stdafx.h"
#include "Engine.h"
#include "UserOutput.h"
#include "KeyboardIO.h"


void PrintHUDText()
{
	UserOutput *pUserOut = EngineManager::Instance()->GetUserOutput();
	KeyboardIO *pKeyboard = EngineManager::Instance()->GetKeyboardIO();
	CTVScreen2DText *pText = EngineManager::Instance()->GetText();

	std::string HUDText = pUserOut->GetOutput();
	std::string HUDInput = pKeyboard->GetOutput();

	pText->Action_BeginText();
	pText->NormalFont_DrawText( HUDText.c_str(), 10, 25.f, cTV_COLORKEY_GREEN );
	pText->NormalFont_DrawText( HUDInput.c_str(), 20, 400, cTV_COLORKEY_GREEN);
	pText->Action_EndText();	
}