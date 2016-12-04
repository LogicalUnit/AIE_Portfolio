/*
	KeyboardIO.h

	This class provides a command-line style interface to the user.
	It is not 100% complete, as it does not provide lower-case characters
	or special symbols, but it is sufficient for our purposes.
*/

#pragma once

class CTVInputEngine;

class KeyboardIO
{
private:
	CTVInputEngine *pInput;
	std::ostringstream chat;
	bool chatting;
	void Backspace();
public:
	KeyboardIO(CTVInputEngine *pInput); //Must be Initialised
	~KeyboardIO(void);

	std::string GetOutput(); //get output for display, including > and _ chars
	std::string GetLine(); //get user input
	
	void Update(); //call in gametick
	void ClearText();
	void StartChatting() { chatting = true; };
	void StopChatting() { chatting = false; };
	bool IsChatting() { return chatting; };
};
