/*
	Player.h
	John Welsby
	2009-5-13

	This hybrid class is used for managing the players (and bots) connected to the game. Each player has an
	associated Actor. The class maintains a list of connected players. Players are identified by a unique name.
*/

#pragma once

#include <list>

class Actor;


class Player
{
//public:
	typedef std::list<Player*> PlayerList;

private:
	Actor *pActor;
	std::string name;

	typedef std::list<Player*> PlayerList;
	static PlayerList players;
	static bool contains(std::string _name);

	Player();

public:
	
	static Player *CreatePlayer(std::string _name, Actor *pActor);
	static Player *GetPlayer(std::string _name);

	//static void DeletePlayer(Player *player);
	static void DeletePlayer(std::string _name);
	
	static void UpdatePlayers(float dt);

	Actor *GetActor(){return pActor;};
	
	~Player();
};
