#include "stdafx.h"
#include "NetworkInterface.h"
#include "MessageTypes.h"
#include "Engine.h"
#include "Player.h"
#include "Actor.h"
#include "Network.h"

void UpdatePosition(std::string name, cTV_3DVECTOR destination)
{
	if(EngineManager::Instance()->GetPlayerName() == name)
		return;

	Player *player = Player::GetPlayer(name);

	if (player == NULL)
	{
		Actor *actor = new Actor( EngineManager::Instance()->GetScene()->CreateActor( name.c_str() ) );
		actor->LoadModel( Actor::GetModelName(9).c_str() );
		player = Player::CreatePlayer(name, actor);
		player->GetActor()->FlipZaxis(true);
	}

	player->GetActor()->SetDestination(destination);
}

void ResolveAttack(std::string attacker, cTV_3DVECTOR target)
{
	static const float ATTACK_RANGE = 30.f;

	Actor *pc = EngineManager::Instance()->GetPlayerCharacter();
	Network *network = EngineManager::Instance()->GetNetwork();

	Player *pAttacker = Player::GetPlayer(attacker);

	if (pAttacker == NULL )
		return;

	pAttacker->GetActor()->SetAttackAnimation();

	cTV_3DVECTOR threat = target - pc->GetPosition();

	float mag = magnitude(threat);
	
	if ( mag < ATTACK_RANGE )
	{
		if ( !pc->IsDying() )
		{

			pc->SetDying();
			
			DyingMessage msg;
			msg.name = EngineManager::Instance()->GetPlayerName();
			strcpy_s(msg.attacker, 64, attacker.c_str());
			network->SendNetMessage(&msg, sizeof(msg));

			//PlaySound(TEXT("Boom.wav"), NULL, SND_FILENAME);
			//Sound should work but it fucking breaks everything
			
		}

	}
}

void KillPlayer(std::string name)
{

	Player *target = Player::GetPlayer(name);

	if (target)
	{
		target->GetActor()->SetDying();
	}

	//Beep(659,200);
}

void RespawnPlayer(std::string name)
{
	Player *target = Player::GetPlayer(name);

	if(target)
		target->GetActor()->SetLiving();

}

//void CreatePlayer(std::string name, std::string modelPath)
//{
//	Player *newPlayer = Player::GetPlayer(name);
//
//	if (newPlayer)
//		return;
//
//	Actor *actor = new Actor( EngineManager::Instance()->GetScene()->CreateActor( name.c_str() ) );
//	actor->LoadModel(modelPath.c_str());
//
//	assert(actor);
//
//	Player::CreatePlayer(name, "NULL", actor);
//}

void SetPlayerModel(std::string name, std::string modelPath)
{
	Player *player = Player::GetPlayer(name);

	if (player == NULL)
		return;

	player->GetActor()->LoadModel( modelPath.c_str() );
}

//void GetModelMessage(ModelMessage *out)
//{
//	out->name = EngineManager::Instance()->GetPlayerName();
//	out->modelPath = EngineManager::Instance()->GetPlayerCharacter()->GetModelPath();
//}

void OnConnect()
{
	Network *network = EngineManager::Instance()->GetNetwork();

	Connect msg;
	strcpy_s( msg.playerName, 64, EngineManager::Instance()->GetPlayerName().c_str() );
	network->SendNetMessage(&msg, sizeof(msg));

	PlaySound(TEXT("/sound/DoorBell.wav"), NULL, SND_FILENAME);
}

void PlayerDisconnect(std::string name)
{
	if (name == EngineManager::Instance()->GetPlayerName() )
		return;

	Player::DeletePlayer(name);
}