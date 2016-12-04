#pragma once

class JCWInputSingleton
{
private:
	JCWInputSingleton(void);
	~JCWInputSingleton(void);
	
	static JCWInputSingleton	*pSingleton;
	CTVInputEngine				*pInput;

public:
	

	static JCWInputSingleton	*GetInstance();
	static void					DeleteInstance();

	bool						IsKeyPressed(cCONST_TV_KEY key);
	void						GetMouseState(int* retRelativeX, int* retRelativeY, 
											bool* retButton1 = NULL, bool* retButton2 = NULL, 
											bool* retButton3 = NULL, bool* retButton4 = NULL, int* retRoll = NULL);
	int							GetMouseX();
	int							GetMouseY();

};

