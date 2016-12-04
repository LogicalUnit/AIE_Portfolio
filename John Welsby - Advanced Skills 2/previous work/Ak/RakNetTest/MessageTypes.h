#pragma once
#include "MessageIdentifiers.h"
enum
{
	ID_CHAT = ID_USER_PACKET_ENUM,
	ID_POSITION
	// More enums....
};

#pragma pack(push, 1)
struct ChatMessage
{
	ChatMessage();
	unsigned char typeId;
	char message[128];
};

struct PositionMessage
{
	PositionMessage();
	unsigned char typeId;
	cTV_3DVECTOR position;
};

#pragma pack(pop)