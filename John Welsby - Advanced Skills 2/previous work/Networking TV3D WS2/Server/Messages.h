#pragma once

struct NetMessage
{	
	NetMessage();
	int ID;	
	char message[128];
};

struct PositionMessage:NetMessage
{
	PositionMessage();
	float posx, posy, posz;
	float facing;
	int	  Uid;
};

struct TauntMessage:NetMessage
{
	TauntMessage();
};
