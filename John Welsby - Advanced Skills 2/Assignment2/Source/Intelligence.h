/*
	Intelligence.h
	John Welsby
	2009-05-13

	This class provides access to the artificial intelligence interface.
	Bots will move randomly around the map according to the waypoints,
	and they will attack the player character if he is in range.
	Bots are indexed by a unique name.
*/

#pragma once
#include "tv_types.h"

class BotManager
{
private:
	typedef std::vector<std::string> BotList;
	typedef std::vector<cTV_3DVECTOR> WaypointList;

	BotList			bots;
	WaypointList	waypoints;

	float	DistanceToPlayer(std::string bot);

public:
	void CreateBot(std::string name, cTV_3DVECTOR position = cTV_3DVECTOR(10, 30, -200) );
	void LoadWaypoints (std::string fileName = std::string("waypoints.txt") );
	void UpdateAI();
	void AttackBots(cTV_3DVECTOR attack);

	cTV_3DVECTOR FindClosestWaypoint(cTV_3DVECTOR position);
	cTV_3DVECTOR FindDestination(cTV_3DVECTOR position);


	BotManager();
	~BotManager();

};