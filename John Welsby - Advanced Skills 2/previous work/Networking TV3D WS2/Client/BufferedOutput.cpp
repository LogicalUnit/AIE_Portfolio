#include "StdAfx.h"
#include "BufferedOutput.h"


BufferedOutput *BufferedOutput::pInstance;


BufferedOutput::BufferedOutput(void)
{
	pText = new CTVScreen2DText();
	ClearBuf();
}

BufferedOutput::~BufferedOutput(void)
{
	if (pText)
		delete pText;
}

void BufferedOutput::AddText(const char *str)
{
	strcat_s(buf, MAX_SIZE, str);
}

const char *BufferedOutput::GetBuf()
{
	return buf;
}

void BufferedOutput::ClearBuf()
{
	memset(buf, 0, MAX_SIZE);
}

void BufferedOutput::PrintText()
{
	pText->Action_BeginText();
	pText->NormalFont_DrawText(buf, 10, 30, cTV_COLORKEY_GREEN);
	pText->Action_EndText();
}

BufferedOutput *BufferedOutput::Instance()
{
	if (pInstance == NULL)
		pInstance = new BufferedOutput();

	return pInstance;
}

void BufferedOutput::Cleanup()
{
	if (pInstance)
		delete pInstance;
}

