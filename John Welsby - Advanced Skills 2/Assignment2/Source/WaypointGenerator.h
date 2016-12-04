/*
	WaypointGenerator.h
	John Welsby
	2009-05-13

	This class is used to dump a set of coordinates to a text file. Simply uncomment the relevant code in main.cpp,
	navigate the player character around the map and press 'O' to dump a waypoint. Intelligence.cpp will read
	in these waypoint and manoeuvre the bots around the level.
*/

#pragma once

#include <fstream>
#include <tv_types.h>

class WaypointGenerator
{
private:
	ofstream output;
	int numWaypoints;

public:
	WaypointGenerator(std::string fileName);
	~WaypointGenerator(void);

	void AddWaypoint(cTV_3DVECTOR waypoint);

	friend ostream& operator<<(ostream &lhs, const cTV_3DVECTOR rhs);
};
