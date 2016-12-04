/*
	UserOutput.h
	John Welsby
	2009-05-13

	This class provides useful functions for displaying output to the user. It maintains a list of string which 
	are displayed sequentially to the user. Significantly, it is used to display network messages to the client.
*/

#pragma once


class UserOutput
{
private:
	static const int NUM_STRINGS = 10;
	std::string lines[NUM_STRINGS];
public:
	UserOutput(void);
	~UserOutput(void);

	std::string GetOutput();

	void AddString(const std::string &line);
	void ClearStrings();
};
