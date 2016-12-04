#ifndef	BUFFEREDOUTPUT_H
#define BUFFEREDOUTPUT_H

class BufferedOutput
{
private:
	static const int MAX_SIZE = 1024;
	char buf[MAX_SIZE];
	CTVScreen2DText *pText;

	BufferedOutput(void);
	~BufferedOutput(void);

	static BufferedOutput *pInstance;

public:
	
	static BufferedOutput *Instance();
	void Cleanup();

	void AddText(const char* str);
	const char *GetBuf();
	void ClearBuf();
	void PrintText();
};

#endif