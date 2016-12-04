#include "StdAfx.h"
#include "UserOutput.h"

UserOutput::UserOutput(void)
{
}

UserOutput::~UserOutput(void)
{
}

void UserOutput::AddString(const std::string &line)
{
	for (int i = NUM_STRINGS - 1; i > 0; i--)
	{
		lines[i] = lines[i-1];
	}
	lines[0] = line;
}

void UserOutput::ClearStrings()
{
	for (int i = 0; i < NUM_STRINGS; i++)
		lines[i] = "";
}

std::string UserOutput::GetOutput()
{
	std::ostringstream output;

	for (int i = 0; i < NUM_STRINGS; i++)
	{
		output<<lines[i];
	}

	return output.str();
}
