#include "stdafx.h"
#include "MessageTypes.h"

NetMessage::NetMessage()
{
	typeId = ID_NET;
}

ChatMessage::ChatMessage (const char *str)
{
	typeId = ID_CHAT;
	strcpy_s(message, 128, str);
};

PositionMessage::PositionMessage (cTV_3DVECTOR vec, std::string name)
{
	typeId = ID_POSITION;
	position = vec;
	strcpy_s(this->name, 64, name.c_str());
};

PlayerDetails::PlayerDetails()
{
	typeId = ID_PLAYER_DETAILS;
	//this->name = name;
	this->ipAddress = "127.0.0.1";
	this->model = "mgolden/AlienLowPoly.tva";
}

RequestPlayerMessage::RequestPlayerMessage()
{
	typeId = ID_REQUEST_PLAYER_DETAILS;
}

AttackMessage::AttackMessage()
{
	typeId = ID_ATTACK;
}

DyingMessage::DyingMessage()
{
	typeId = ID_DYING;
}

RespawnMessage::RespawnMessage()
{
	typeId = ID_RESPAWN;
}

ModelMessage::ModelMessage()
{
	typeId = ID_MODEL_INFO;
}
Disconnect::Disconnect()
{
	typeId = ID_DISCONNECT;
}

Connect::Connect()
{
	typeId = ID_CONNECT;
}