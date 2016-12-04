#include "stdafx.h"
#include "Engine.h"
#include "Player.h"
#include "Actor.h"
#include "Intelligence.h"
#include "NetworkInterface.h"

#include <fstream>

BotManager::BotManager()
{

}

BotManager::~BotManager()
{

}

void BotManager::CreateBot(std::string name, cTV_3DVECTOR position)
{
	Actor *newActor = new Actor ( EngineManager::Instance()->GetScene()->CreateActor( name.c_str() ) );

	assert(newActor);

	Player *newBot = Player::CreatePlayer( name, newActor );

	if (newBot == NULL) //name already used
	{
		delete newActor;
		return;
	}

	newActor->SetPosition(position);
	newActor->SetDestination(position);
	newActor->LoadModel( Actor::GetModelName(9).c_str() );
	newActor->SetSpeed(0.25);
	newActor->FlipZaxis(true);

	bots.push_back(name);
}

//This fn does not do any format checking. Only use on correctly formatted data!!!

inline cTV_3DVECTOR LineToVector(std::string line)
{
	int index = 0;
	float x = 0.f, z = 0.f;

	sscanf_s(line.c_str(), "%d x: %f z: %f", &index, &x, &z);

	return cTV_3DVECTOR(x, 30.f, z);
}

void BotManager::LoadWaypoints(std::string fileName)
{
	ifstream input(fileName.c_str());

	string line;

	while ( getline(input, line, '\n') )
	{
		waypoints.push_back( LineToVector(line) );
	}

}

cTV_3DVECTOR BotManager::FindClosestWaypoint(cTV_3DVECTOR position)
{
	float smallest = 10e10f;
	cTV_3DVECTOR result = position;

	for (unsigned int i = 0; i < waypoints.size(); i++)
	{
		float mag = magnitude(waypoints[i] - position);
		if (mag < smallest)
		{
			smallest = mag;
			result = waypoints[i];
		}
	}

	return result;
}

cTV_3DVECTOR BotManager::FindDestination(cTV_3DVECTOR position)
{
	WaypointList possibleDestinations;
	cTV_3DVECTOR result = position;
	float flimit = 20.f, climit = 800.f;

	for (unsigned int i = 0; i < waypoints.size(); i++)
	{
		float mag = magnitude(waypoints[i] - position);

		if (mag <= climit && mag >= flimit)
		{
			possibleDestinations.push_back(waypoints[i]);
		}
	}

	if (possibleDestinations.size() > 0 )
	{
		int choice = rand() % possibleDestinations.size();
		result = possibleDestinations[choice];
	}

	return result;
}

void BotManager::UpdateAI()
{
	for (unsigned int i = 0; i < bots.size(); i++)
	{
		Player *pBot = Player::GetPlayer(bots[i]);

		if (pBot == NULL)
			return;


		cTV_3DVECTOR target = EngineManager::Instance()->GetPlayerCharacter()->GetPosition();
		cTV_3DVECTOR position = pBot->GetActor()->GetPosition();
		float distance = magnitude(target - position);

		if (distance <= 50 && EngineManager::Instance()->GetPlayerCharacter()->GetAnimation() != 3 ) //player not dying
		{
			if( !pBot->GetActor()->IsDying() )
			{
				pBot->GetActor()->SetAttackAnimation();
				ResolveAttack(bots[i], target);
			}
		}
		
		if (pBot->GetActor()->GetAnimation() == 1 ) //idle
		{
			cTV_3DVECTOR destination = FindDestination( pBot->GetActor()->GetPosition() );
			pBot->GetActor()->SetDestination( destination );
		}
	}
}

float BotManager::DistanceToPlayer(std::string name)
{
	Player *pBot = Player::GetPlayer(name);

	cTV_3DVECTOR distance = EngineManager::Instance()->GetPlayerCharacter()->GetPosition() - pBot->GetActor()->GetPosition();

	float mag = magnitude(distance);

	return mag;
}

void BotManager::AttackBots(cTV_3DVECTOR attack)
{
	for (unsigned int i = 0; i < bots.size(); i++)
	{
		Player *pBot = Player::GetPlayer(bots[i]);

		if (pBot)
		{
			float range = magnitude( pBot->GetActor()->GetPosition() - attack);
			if (range <= 40)
				pBot->GetActor()->SetDying();
		}
			
	}
}