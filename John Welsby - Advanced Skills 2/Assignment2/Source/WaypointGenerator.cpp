#include "StdAfx.h"
#include "WaypointGenerator.h"

WaypointGenerator::WaypointGenerator(std::string fileName) : numWaypoints(0)
{
	output.open(fileName.c_str());
}

WaypointGenerator::~WaypointGenerator(void)
{
}

void WaypointGenerator::AddWaypoint(cTV_3DVECTOR waypoint)
{
	output<<numWaypoints<<" "<<waypoint<<endl;
	numWaypoints++;
}

ostream &operator<<(ostream &lhs, const cTV_3DVECTOR rhs)
{
	lhs<<"x: "<<rhs.x<<" z: "<<rhs.z;
	return lhs;
}