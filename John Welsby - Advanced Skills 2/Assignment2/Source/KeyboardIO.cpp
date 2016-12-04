#include "StdAfx.h"
#include "CTVMiniMesh.h" //needed before CTVInputEngine for the "byte" typedef. . . 
#include "CTVInputEngine.h"
#include "KeyboardIO.h"


typedef unsigned char byte;

KeyboardIO::KeyboardIO(CTVInputEngine *pInput) : pInput(pInput), chatting(false)
{
}

KeyboardIO::~KeyboardIO(void)
{
}

void KeyboardIO::Backspace()
{
	std::string temp = chat.str();
	size_t len = temp.length();

	if (len > 0)
		temp.resize(len - 1);

	ClearText();
	chat<<temp;
}
void KeyboardIO::Update()
{
	if (!chatting)
		return;

	static bool pressed = false;
	static int keyDown = 0;
	static unsigned char keys[255];

	pInput->GetKeyPressedArray((byte*)keys);

	for (int i = 0; i < 255; i++)
	{
		if(keys[i] && !pressed) //key pressed
		{
			pressed = true;
			keyDown = i;	

			char c = pInput->GetASCIIFromKey((cCONST_TV_KEY)keyDown);

			if ((cCONST_TV_KEY)keyDown == cTV_KEY_BACKSPACE)
			{
				Backspace();	
			}
			if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == ' ') || (c == '.') )
			//if ( c != '\r' )
			{		
				chat<<c;
			}
		}
	}

	if (!pInput->IsKeyPressed( (cCONST_TV_KEY)keyDown )) //key released
	{
		pressed = false;
	}
	
}


std::string KeyboardIO::GetLine()
{
	return chat.str();
}

void KeyboardIO::ClearText()
{
	chat.str("");
}

std::string KeyboardIO::GetOutput()
{
	std::ostringstream finalOutput;

	//finalOutput.str("");

	if (chatting)
		finalOutput<<'>';

	finalOutput<<chat.str();

	if (chatting)
			finalOutput<<'_';

	return finalOutput.str();
}