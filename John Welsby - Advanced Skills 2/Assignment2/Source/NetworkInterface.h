#pragma once
#include "tv_types.h"

void SetPlayerModel(std::string name, std::string modelPath);

void UpdatePosition(std::string name, cTV_3DVECTOR destination);

void ResolveAttack(std::string attacker, cTV_3DVECTOR target);

void KillPlayer(std::string name);

void RespawnPlayer(std::string name);

void OnConnect();

void PlayerDisconnect(std::string name);
