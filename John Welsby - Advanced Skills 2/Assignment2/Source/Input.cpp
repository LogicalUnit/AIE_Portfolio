#include "stdafx.h"
#include "Engine.h"
#include "Network.h"
#include "KeyboardIO.h"
#include "UserOutput.h"
#include "Actor.h"
#include "Player.h"
#include "Intelligence.h"

static BotManager *botManager = new BotManager(); //TODO: make this better!

extern bool done;

static float heartbeat = 500.f;

/***************************************/
void Input(float delta)
{
	//Engine Interface
	CTVInputEngine	*pInput = EngineManager::Instance()->GetInput();
	CTVCamera		*pCamera = EngineManager::Instance()->GetCamera();
	Network			*pNet = EngineManager::Instance()->GetNetwork();
	KeyboardIO		*pKeyboard = EngineManager::Instance()->GetKeyboardIO();
	UserOutput		*pUserOut = EngineManager::Instance()->GetUserOutput();
	Actor			*pPlayerCharacter = EngineManager::Instance()->GetPlayerCharacter();


	cTV_3DVECTOR oldPos = pPlayerCharacter->GetPosition(); //for collisions

	static bool returnDown = false;

	if(pInput->IsKeyPressed(cTV_KEY_RETURN) && !returnDown)
	{
		returnDown = true;

		if ( pKeyboard->IsChatting() )
		{
			std::string line = pKeyboard->GetLine();
			pKeyboard->ClearText();

			if (!line.empty())
			{
				stringstream input(line);
				string command;	
				getline(input, command, ' ');
				
				if (command == "NAME")
				{
					string param;
					getline(input, param, ' ');
					EngineManager::Instance()->SetPlayerName(param);
				}
				else if (command == "CONNECT")
				{
					string param;
					getline(input, param, ' ');
					pNet->SetIPAddress(param);
					pNet->StartClient();					
				}
				else if(command == "MODEL")
				{
					string param;
					getline(input, param, ' ');
					int model = atoi(param.c_str());
					
					EngineManager::Instance()->GetPlayerCharacter()->LoadModel( Actor::GetModelName(model).c_str() );

					ModelMessage msg;
					msg.name = EngineManager::Instance()->GetPlayerName();

					strcpy_s(msg.modelPath, 64, EngineManager::Instance()->GetPlayerCharacter()->GetModelPath().c_str() );

					pNet->SendNetMessage(&msg, sizeof(msg));
				}
				else if (command == "BOT")
				{
					static bool initialised = false;

					if (!initialised)
					{
						botManager->LoadWaypoints();
						initialised = true;
					}
					
					string param;
					getline(input, param, ' ');
					botManager->CreateBot(param);

					PlaySound(TEXT("/sound/Calculat.wav"), NULL, SND_FILENAME);

				}
				else if (command == "QUIT")
				{
					Disconnect msg;
					strcpy_s( msg.playerName, 64, EngineManager::Instance()->GetPlayerName().c_str() );
					pNet->SendNetMessage(&msg, sizeof(msg));
					PlaySound(TEXT("/sound/Goodbye.wav"), NULL, SND_FILENAME);
					done = true;
				}
				else if (command == "HEARTBEAT")
				{
					string param;
					getline(input, param, ' ');
					float beat = (float)atof(param.c_str());
					if (beat >= 100 && beat <= 2000)
					{
						heartbeat = beat;
						PlaySound(TEXT("/sound/Thump.wav"), NULL, SND_FILENAME);
					}
				}
				else
				{	
					ChatMessage msg(line.c_str());
					pNet->SendNetMessage(&msg, sizeof(ChatMessage));
				}
			}

			pKeyboard->StopChatting();			
		}
		else
		{
			pKeyboard->StartChatting();
		}

	}
	
	pKeyboard->Update();

	if(!pInput->IsKeyPressed(cTV_KEY_RETURN))
	{
		returnDown = false;
	}
	
	if ( !pKeyboard->IsChatting() && !pPlayerCharacter->IsDying() )
	{	
		static bool eDown = false;
		static float attackDelay = 0.f;
		static bool running = false;

		if (pInput->IsKeyPressed(cTV_KEY_E) && !eDown )
		{
			eDown = true;
			AttackMessage attack;
			attack.attacker = EngineManager::Instance()->GetPlayerName();
			attack.target = pPlayerCharacter->GetAttackVector();
			botManager->AttackBots(attack.target);
			pPlayerCharacter->SetAttackAnimation();
			pNet->SendNetMessage(&attack, sizeof(attack));
			attackDelay = 600.f;
			running = false;
		}

		if (attackDelay > 0)
			attackDelay -= delta;

		if (!pInput->IsKeyPressed(cTV_KEY_E))
			eDown = false;

		static bool wDown = false;

		if (pInput->IsKeyPressed(cTV_KEY_W))
		{
			if ( attackDelay <= 0 )
			{
				pPlayerCharacter->MoveForward(delta);

				if (!wDown || !running )
				{
					pPlayerCharacter->SetRunningAnimation();
					running = true;
				}
			}
			wDown = true;
		}

		
		if (!pInput->IsKeyPressed(cTV_KEY_W) && wDown )
		{
			pPlayerCharacter->SetIdleAnimation();
			wDown = false;
		}

		if (pInput->IsKeyPressed(cTV_KEY_S))
			pPlayerCharacter->MoveBackward(delta);

		if (pInput->IsKeyPressed(cTV_KEY_A))
			pPlayerCharacter->PivotRight(delta);
		
		if (pInput->IsKeyPressed(cTV_KEY_D))
			pPlayerCharacter->PivotLeft(delta);


		static bool mDown = false;

		if (pInput->IsKeyPressed(cTV_KEY_M) && !mDown)
		{
			ChatMessage msg("John Rules!");
			pNet->SendNetMessage(&msg, sizeof(ChatMessage));
			mDown = true;
		}

		if (!pInput->IsKeyPressed(cTV_KEY_M))
			mDown = false;

	}

	cTV_3DVECTOR camPos = pPlayerCharacter->GetChaseCamPos();
	cTV_3DVECTOR playerPos = pPlayerCharacter->GetPosition();


	pCamera->SetPosition(camPos.x, camPos.y, camPos.z);
	pCamera->SetLookAt(playerPos.x, playerPos.y+60, playerPos.z);

	
	if (EngineManager::Instance()->GetScene()->Collision(&playerPos, &oldPos)) //actor collision
	{	
		pPlayerCharacter->SetPosition(oldPos);
	}


	
	if ( pPlayerCharacter->IsDying() )
	{
		pPlayerCharacter->CountDownToSpawn(delta);

		if( !pPlayerCharacter->IsDying() )
		{
			PlaySound(TEXT("/sound/DeadRing.wav"), NULL, SND_FILENAME);

			RespawnMessage msg;
			msg.name = EngineManager::Instance()->GetPlayerName();
			pNet->SendNetMessage(&msg, sizeof(msg));	
		}
	}

	static float totalTime = 0;
	totalTime += delta;
	
	if (totalTime >= heartbeat && !pPlayerCharacter->IsDying() ) //send heartbeat to server
	{
		totalTime = 0;
		PositionMessage msg(playerPos, EngineManager::Instance()->GetPlayerName() );
		pNet->SendNetMessage(&msg, sizeof(msg));
	}

	static float intelligenceTime = 0;
	intelligenceTime += delta;

	if (intelligenceTime > 500) //update bot behavior
	{
		botManager->UpdateAI();
		intelligenceTime = 0;
	}
}
