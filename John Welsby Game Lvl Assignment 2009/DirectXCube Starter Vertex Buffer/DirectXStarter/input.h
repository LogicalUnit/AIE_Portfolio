#pragma once
#include "dinput.h"

class Input
{
public:
	Input(void);
	~Input(void);

	void Update();

	//Example keys: DIK_A, DIK_SPACE
	bool IsKeyDown( int KEY );
private:
	//DirectX Handling vars
	LPDIRECTINPUT8			m_lpdi;
	LPDIRECTINPUTDEVICE8	m_keyboard;
	char					ma_KeyboardState[256]; 

	bool bFailedToInit;
};
