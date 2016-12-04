/*
	Actor.h
	John Welsby
	2009-5-13

	Wrapper class for CTVActor. Provides easier functionality, and include time-dependent move/update functions.
	Must be constructed with a valid actor pointer. This pointer is deleted in the destructor, so it must
	be called BEFORE DestroyAllMeshes()
*/

#pragma once
#include <tv_types.h>

class CTVActor;
class CTVMathLibrary;

class Actor
{
private:
	
	enum
	{
		ANIMATION_NONE,
		ANIMATION_IDLE,
		ANIMATION_RUNNING,
		ANIMATION_DYING,
		ANIMATION_ATTACKING
	} anim;

	CTVActor		*pActor;
	float			speed;
	float			pivotSpeed;
	float			timeToSpawn;
	float			timeToAttack;
	bool			flipZ;
	bool			dying;
	cTV_3DVECTOR	destination;
	std::string		modelPath;

	Actor(void);

public:
	Actor(CTVActor *_pActor); //must be created with CTVScene::CreateActor()
	~Actor(void);

	void LoadModel(const char *mesh);

	void SetSpeed(float speed) {this->speed = speed;};
	void SetPivotSpeed(float speed) {this->pivotSpeed = speed;};
	void SetDestination(cTV_3DVECTOR destination) {this->destination = destination;};
	void FlipZaxis(bool flip = false) { this->flipZ = flip; };
	
	void MoveForward(float dt);
	void MoveBackward(float dt);
	void PivotLeft(float dt);
	void PivotRight(float dt);
	void MoveToDestination(float dt);
	void Update(float dt);

	void SetIdleAnimation();
	void SetWalkingAnimation();
	void SetRunningAnimation();
	void SetDyingAnimation();
	void SetAttackAnimation();

	void SetDying();
	void SetLiving();
	void CountDownToSpawn(float dt);
	
	void MoveRelative(float x, float y, float z);
	void SetPosition (cTV_3DVECTOR pos);
	void SetRotation (float x, float y, float z);
	
	cTV_3DVECTOR GetChaseCamPos();
	cTV_3DVECTOR GetPosition();
	cTV_3DVECTOR GetAttackVector();

	bool	IsDying(){return dying;};
	int		GetAnimation(){return anim;}

	static std::string	GetModelName(int modelId);
	std::string			GetModelPath(){return modelPath;};

	CTVMathLibrary *GetMaths();


};
