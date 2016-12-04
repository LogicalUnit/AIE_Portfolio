/*
	MessageTypes.h
	John Welsby
	2009-5-13
	This class declares the various message structures used by the program. Each message has a unique identifier,
	and additional data to be transferred over the network
*/

#pragma once
#include "MessageIdentifiers.h"
#include <tv_types.h>
enum
{
	ID_NET = ID_USER_PACKET_ENUM,
	ID_CHAT,
	ID_POSITION,
	ID_PLAYER_DETAILS,
	ID_REQUEST_PLAYER_DETAILS,
	ID_ATTACK,
	ID_DYING,
	ID_RESPAWN,
	ID_MODEL_INFO,
	ID_DISCONNECT,
	ID_CONNECT
	// More enums....
};

#pragma pack(push, 1)

struct NetMessage
{
	NetMessage();
	unsigned char typeId;
};

struct ChatMessage:NetMessage
{
	ChatMessage(const char *str);
	char message[128];
};

struct PositionMessage:NetMessage
{
	PositionMessage(cTV_3DVECTOR vec, std::string name);
	cTV_3DVECTOR position;
	//std::string name;
	char name[64];
};

struct PlayerDetails:NetMessage
{
	PlayerDetails();
	std::string name;
	std::string ipAddress;
	std::string model;
};

struct RequestPlayerMessage:NetMessage
{
	RequestPlayerMessage();
};

struct AttackMessage:NetMessage
{
	AttackMessage();
	std::string attacker;
	cTV_3DVECTOR target;
};

struct DyingMessage:NetMessage
{
	DyingMessage();
	std::string name;
	char attacker[64];
};

struct RespawnMessage:NetMessage
{
	RespawnMessage();
	std::string name;
};

struct ModelMessage:NetMessage
{
	ModelMessage();
	std::string name;
	//std::string modelPath;
	char modelPath[64];
};

struct Disconnect:NetMessage
{
	Disconnect();
	char playerName[64];
};

struct Connect:NetMessage
{
	Connect();
	char playerName[64];
};

#pragma pack(pop)