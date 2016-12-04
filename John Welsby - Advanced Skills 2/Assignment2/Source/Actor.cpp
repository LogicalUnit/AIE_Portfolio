#include "StdAfx.h"
#include "Actor.h"
#include "CTVActor.h"
#include "CTVMathLibrary.h"



Actor::Actor(CTVActor *_pActor) : pActor(_pActor), flipZ(false), speed(0.25f), pivotSpeed(0.003f), anim(ANIMATION_NONE), dying(false), timeToSpawn(0.0f), timeToAttack(0.0f)
{
	pActor->SetCollisionEnable(false);
	SetIdleAnimation();
}


Actor::~Actor(void)
{
	if(pActor)
		pActor->Destroy(); //Must be called BEFORE the SceneManager is destroyed

	pActor = NULL;
}

void Actor::LoadModel(const char *mesh) //can be called on-the-fly to change models
{
	if ( pActor->LoadTVA(mesh) )
		modelPath = mesh;
}

void Actor::MoveRelative(float x, float y, float z)
{
	cTV_3DVECTOR pos = pActor->GetPosition();
	pos.x += x;
	pos.y += y;
	pos.z += z;
	pActor->SetPosition(pos.x, pos.y, pos.z);
}

void Actor::PivotRight(float dt)
{
	if(dying)
		return;

	pActor->RotateY(-pivotSpeed * dt);
}

void Actor::PivotLeft(float dt)
{
	if(dying)
		return;

	pActor->RotateY(pivotSpeed * dt);
}

void Actor::SetIdleAnimation()
{
	pActor->SetAnimationByName("idle");
	pActor->SetAnimationLoop(true);
	pActor->PlayAnimation();
	anim = ANIMATION_IDLE;
}

void Actor::SetWalkingAnimation()
{
	pActor->SetAnimationByName("walk");
	pActor->SetAnimationLoop(true);
	pActor->PlayAnimation();
}

void Actor::SetRunningAnimation()
{
	pActor->SetAnimationByName("run");
	pActor->SetAnimationLoop(true);
	pActor->PlayAnimation();
	anim = ANIMATION_RUNNING;
}

void Actor::SetDyingAnimation()
{
	pActor->SetAnimationByName("Die");
	pActor->SetAnimationLoop(false);
	pActor->PlayAnimation();
	anim = ANIMATION_DYING;
}

void Actor::SetAttackAnimation()
{
	pActor->SetAnimationByName("Attack");
	pActor->SetAnimationLoop(false);
	pActor->PlayAnimation();
	timeToAttack = 600.f;
}

void Actor::SetPosition(cTV_3DVECTOR pos)
{
	pActor->SetPosition(pos.x, pos.y, pos.z);
}

void Actor::SetRotation(float x, float y, float z)
{
	pActor->SetRotation(x, y, z);
}

void Actor::MoveToDestination(float dt) //major function used for all NPC movement. Inludes animation logic
{
	if(dying)
		return;

	float mag = magnitude( pActor->GetPosition() - destination );

	if ( mag < 0.1f )
	{
		if (anim != ANIMATION_IDLE)
		{
			SetIdleAnimation();
			anim = ANIMATION_IDLE;
		}
		return;
	}
	
	CTVMathLibrary *maths = GetMaths();
	
	pActor->LookAtPoint(&destination, true);
	cTV_3DVECTOR dir = destination - pActor->GetPosition();
	maths->TVVec3Normalize(&dir, &dir);
	dir *= speed * dt;
	cTV_3DVECTOR final = dir + pActor->GetPosition();
	
	if (flipZ)
		pActor->RotateY(3.14159f); //because the model faces down the wrong z-axis
	
	pActor->SetPosition(final.x, final.y, final.z);

	if (anim != ANIMATION_RUNNING)
	{
		SetRunningAnimation();
		anim = ANIMATION_RUNNING;
	}
}

cTV_3DVECTOR Actor::GetChaseCamPos()
{
	cTV_3DMATRIX rot = pActor->GetRotationMatrix();
	cTV_3DVECTOR forward(rot.m31, rot.m32, rot.m33);
	cTV_3DVECTOR camPos = forward * -100; //distance behind to set camera

	if (flipZ)
		camPos *= -1;

	camPos += pActor->GetPosition();
	camPos.y += 100;

	return camPos;
}

cTV_3DVECTOR Actor::GetAttackVector()
{
	cTV_3DMATRIX rot = pActor->GetRotationMatrix();
	cTV_3DVECTOR forward(rot.m31, rot.m32, rot.m33);
	cTV_3DVECTOR attack = forward * 50;

	if (flipZ)
		attack *= -1;

	attack += pActor->GetPosition();

	return attack;
}

cTV_3DVECTOR Actor::GetPosition()
{
	return pActor->GetPosition();
}

void Actor::MoveForward(float dt)
{
	if(dying)
		return;

	if (flipZ)
		pActor->RotateY(3.14159f);
	
	cTV_3DMATRIX rot = pActor->GetRotationMatrix();
	cTV_3DVECTOR forward(rot.m31, rot.m32, rot.m33);
	forward *= speed * dt;
	cTV_3DVECTOR final = forward + pActor->GetPosition();
	pActor->SetPosition(final.x, final.y, final.z);

	if (flipZ)
		pActor->RotateY(3.14159f);
	
}

void Actor::MoveBackward(float dt)
{
	if(dying)
		return;

	if (flipZ)
		pActor->RotateY(3.14159f);
	
	cTV_3DMATRIX rot = pActor->GetRotationMatrix();
	cTV_3DVECTOR forward(rot.m31, rot.m32, rot.m33);
	forward *= -speed * dt;
	cTV_3DVECTOR final = forward + pActor->GetPosition();
	pActor->SetPosition(final.x, final.y, final.z);

	if (flipZ)
		pActor->RotateY(3.14159f);
}



void Actor::SetDying()
{
	static const float RESPAWN_DELAY = 3500.f;

	dying = true;
	timeToSpawn = RESPAWN_DELAY;
	SetDyingAnimation();
}

void Actor::SetLiving()
{
	dying = false;
	SetIdleAnimation();
}

void Actor::CountDownToSpawn(float dt)
{
	timeToSpawn -= dt;

	if (timeToSpawn <= 0 && dying)
		SetLiving();
}

std::string Actor::GetModelName(int modelId)
{
	switch(modelId)
	{
	case 0:
		return "tfraser/Tfraser_lowpoly.tva";
	case 1:
		return "cleed/Cleed_Animations.TVA";
	case 2:
		return "mplatt/mplatt_lowpolychar.tva";
	case 3:
		return "CMcBride TVA/Macebeard.tva";
	case 4:
		return "Cmernagh/CMernagh_Toddanimlow.tva";
	case 5:
		return "FMorland TVA/fmorland_chariter.tva";
	case 6:
		return "Jevans/jevans_charlie_tvexport.tva";
	case 7:
		return "Josh cooper TVA/Baby.tva";
	case 8:
		return "TimTVA/tfletcher_TVA export.tva";
	default:
		return "mgolden/AlienLowPoly.tva";
	}
}

CTVMathLibrary *Actor::GetMaths()
{
	static CTVMathLibrary *maths = new CTVMathLibrary();

	return maths;
}

void Actor::Update(float dt) //called in gametick
{
	if (dying)
		CountDownToSpawn(dt);

	if(timeToAttack > 0)
	{
		timeToAttack -= dt;
	}
	else 
	{
		MoveToDestination(dt);
	}

}