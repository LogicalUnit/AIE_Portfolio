#include "stdafx.h"
#include "Player.h"
#include "Actor.h"


Player::PlayerList Player::players;

Player::Player() : pActor(NULL)
{

}

Player::~Player()
{
	if(pActor)
		delete pActor;
}


bool Player::contains(std::string _name)
{
	for (PlayerList::iterator it = players.begin(); it != players.end(); it++)
	{
		if  ( ( *it )->name == _name)
			return true;
	}

	return false;
}

Player *Player::CreatePlayer(std::string _name, Actor *_pActor)
{
	if (contains(_name))
		return NULL;

	Player *player = new Player();
	player->name = _name;
	player->pActor = _pActor;
	player->GetActor()->SetPosition(cTV_3DVECTOR(10, 30, -200));
	players.push_back(player);


	return player;
}

Player *Player::GetPlayer(std::string _name)
{
	for (PlayerList::iterator it = players.begin(); it != players.end(); it++)
	{
		if  ( ( *it )->name == _name)
			return (*it);
	}

	return NULL;
}

//Not used, but will leave just in case
//void Player::DeletePlayer(Player *player)
//{
//	if (player == NULL)
//		return;
//
//	for (PlayerList::iterator it = players.begin(); it != players.end(); it++)
//	{
//		if ( ( *it )->name == player->name )
//		{
//			players.remove(player);
//		}
//	}
//}

void Player::DeletePlayer(std::string _name)
{
	for (PlayerList::iterator it = players.begin(); it != players.end(); it++)
	{
		if ( ( *it )->name == _name )
		{
			Player *player = *it;
			delete player;
			players.remove( player );
			break;
		}
	}
}

void Player::UpdatePlayers(float dt) //call in gametick
{
	for(Player::PlayerList::iterator it = Player::players.begin(); it != Player::players.end(); it++)
	{
		( *it )->GetActor()->Update(dt);
	}
}

