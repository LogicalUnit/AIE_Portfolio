#include "stdafx.h"

NetMessage::NetMessage()
{
	ID = 601;
}

PositionMessage::PositionMessage()
{
	ID = 602;	
	//pos = 0;
	facing = 0;
	Uid = 1;
};

TauntMessage::TauntMessage()
{
	ID = 603;
}

