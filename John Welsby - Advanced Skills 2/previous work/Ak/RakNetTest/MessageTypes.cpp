#include "stdafx.h"

ChatMessage::ChatMessage ()
{
	typeId = ID_CHAT;
	strcpy_s(message, 128, "John Rules!");
};

PositionMessage::PositionMessage ()
{
	typeId = ID_POSITION;
	position = cTV_3DVECTOR(0, 0, 0);
};