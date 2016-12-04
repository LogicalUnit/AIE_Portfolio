
#include "stdafx.h"

Input::Input(void)
{
	bFailedToInit = false;
	//Create DirectInput Device
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
								   IID_IDirectInput8, (void**)&m_lpdi, NULL)))
	{
		OutputDebugString("Unable to create Direct Input. \n");
		bFailedToInit = true;
		return;
	}

	//Create Keyboard Device  
	if (FAILED(m_lpdi->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL)))
    {
		OutputDebugString("Unable to create Direct Input Keyboard Device. \n");
		bFailedToInit = true;
		return;
	}

	//Set Data Format, at this point, you could make it mouse, joystick etc...  
	if (FAILED(m_keyboard->SetDataFormat(&c_dfDIKeyboard)))
    {
		OutputDebugString("Unable to Set Keyboard Data Format. \n");
		bFailedToInit = true;
		return;
	}
	//Set window cooperation, this defines how the input handle we have interact with other applications.
	// In this case, we are telling DirectInput that we want exclusive access and that this access should 
	// only be valid if the application is in the foreground. As our application moves to the background, 
	// the device is automatically unacquired.
	// This will send a fail message initially when run from visual studio in windowed mode 
	// but the update function will re-acquire the input. The fail would not occur if run from an executable and in full screen mode

	if (FAILED(m_keyboard->SetCooperativeLevel(0, DISCL_FOREGROUND| DISCL_EXCLUSIVE ))) 
    {
		OutputDebugString("Unable to Set Cooperative Level... \n");
		bFailedToInit = true;
		return;
	}

	// this gets the input for our application so we can begin querying it
	if (FAILED(m_keyboard->Acquire()))
    {
		OutputDebugString("Unable to Acquire Keyboard. \n");
		bFailedToInit = true;
		return;
	}
}

Input::~Input(void)
{
    if (m_lpdi) 
    { 
        if (m_keyboard) 
        { 
        // Always unacquire device before calling Release(). 
            m_keyboard->Unacquire(); 
			// release pointer - not delete - it is not our pointer to delete
            m_keyboard->Release();
            m_keyboard = NULL; 
        } 
		// release pointer - not delete - it is not our pointer to delete
        m_lpdi->Release();
        m_lpdi = NULL; 
    } 
}

void Input::Update()
{
	HRESULT hr;

	// clear the buffer we use for retrieving input
	ZeroMemory( ma_KeyboardState, sizeof(ma_KeyboardState));
	
	// fill the buffer with the state of the keyboard
    if(FAILED(m_keyboard->GetDeviceState(sizeof(ma_KeyboardState),(LPVOID)&ma_KeyboardState)))
    {
		// if the input has been aquired by another window try to re acquire it
		OutputDebugString("Failed to get Keyboard Buffer. \n");
		hr = m_keyboard->Acquire();
        
		// keep trying
		while( hr == DIERR_INPUTLOST || hr == DIERR_OTHERAPPHASPRIO)
			hr = m_keyboard->Acquire();
    } 
}

bool Input::IsKeyDown(int Key)
{
	return (ma_KeyboardState[Key] && 0x80);
}