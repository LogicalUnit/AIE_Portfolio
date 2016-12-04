#include "StdAfx.h"
#include "Object.h"
#include <math.h>
#include <sstream>
#include <string>

Object::Object( CTVScene *pScene )
{
	this->pScene = pScene;	
	pText = new CTVScreen2DText();
	pMath = new CTVMathLibrary();

	// create actors
	AddActor( cTV_3DVECTOR( 0, 0, 0 ) );
	AddActor( cTV_3DVECTOR( 100, 0, 100 ) );
	AddActor( cTV_3DVECTOR( 175, 0, 100 ) );

	
	AddActor( cTV_3DVECTOR( 0, 175, 0 ) );
	angle3= 0.0f;
	spyroPos = cTV_4DVECTOR( 0, 175, 0, 0 );
}

Object::~Object(void)
{
	if( !ActorList.empty() )
	{
		std::vector<CTVActor*>::iterator start, end;
		start = ActorList.begin();
		end = ActorList.end();

		for( start; start != end; start++ )
		{
			(*start)->Destroy();
			(*start) = NULL ;
		}

		start = ActorList.begin();
		ActorList.erase( start, end );
	}

	if( pMath )
	{
		delete( pMath );
		pMath = NULL;
	}

	if( pText )
	{
		delete( pText );
		pText = NULL;
	}
}


void Object::Update( float dt )
{	

	// set up agle and matrix each loop to adjust for computer speed
	angle1 = 0.001f * dt;
	angle2 = 0.001f * dt;
	angle3 += 0.0004f * dt;

	rotYMatrix = cTV_3DMATRIX( cos(angle1), 0, -sin(angle1), 0,
							             0, 1,            0, 0,
							   sin(angle1), 0,  cos(angle1), 0,
							             0, 0,            0, 1 );

	invRotYMatrix = cTV_3DMATRIX( cos(-angle1), 0, -sin(-angle1), 0,
							             0, 1,            0, 0,
							   sin(-angle1), 0,  cos(-angle1), 0,
							             0, 0,            0, 1 );

	rotYMatrixFaster = cTV_3DMATRIX( cos(3*angle1), 0, -sin(3*angle1), 0,
							             0, 1,            0, 0,
							   sin(3*angle1), 0,  cos(3*angle1), 0,
							             0, 0,            0, 1 );
		
		
	rotXMatrix = cTV_3DMATRIX(  1,            0,           0, 0,
							    0,  cos(angle2), sin(angle2), 0,
							    0, -sin(angle2), cos(angle2), 0,
							    0,            0,           0, 1 );

	rotXMatrix2 = cTV_3DMATRIX(  1,            0,           0, 0, //different angle for spyro
							    0,  cos(angle3), sin(angle3), 0,
							    0, -sin(angle3), cos(angle3), 0,
							    0,            0,           0, 1 );


	rotZMatrix = cTV_3DMATRIX( cos(angle1), -sin(angle1), 0, 0,
							   sin(angle1),  cos(angle1), 0, 0,
									     0,			   0, 1, 0,
										 0,			   0, 0, 1 );


	// get the actors current position
	cTV_3DVECTOR pos[3];
	pos[0] = ActorList[1]->GetPosition(); // First orbiting actor, called "planet"
	pos[1] = ActorList[2]->GetPosition() - ActorList[1]->GetPosition(); //moon. Translate back to origin.
	//pos[2] = ActorList[3]->GetPosition(); //spyro

	// convert actor position to a 4d vector 
	// and make a 4d vector for our new position

	static const int numActors = 3;

	cTV_4DVECTOR newPos[numActors], oldPos[numActors];
	
	for (int i = 0; i < numActors; i++)
	{
		oldPos[i].x = pos[i].x, 
		oldPos[i].y = pos[i].y, 
		oldPos[i].z = pos[i].z,
		oldPos[i].w = 0;
	}
	
	////////// rotation ///////////

	// get the current rotation matrix of the actor
	rotation[0] = ActorList[1]->GetRotationMatrix();
	rotation[1] = ActorList[2]->GetRotationMatrix();
	rotation[2] = ActorList[3]->GetRotationMatrix();
	
	// multiply it by the rotation matrix we want to use
	pMath->TVMatrixMultiply( &rotation[0], &rotation[0], &invRotYMatrix ); //planet

	pMath->TVMatrixMultiply( &rotation[1], &rotation[1], &rotYMatrixFaster ); //moon

	pMath->TVMatrixMultiply( &rotation[2], &rotation[2], &rotZMatrix ); //spyro

	///////// end rotation ////////

	////////// position ///////////
		
	// use the math library transform function to do the multiplication for us
	pMath->TVVec4Transform( &newPos[0], &oldPos[0], &rotYMatrix ); //planet

	pMath->TVVec4Transform( &newPos[1], &oldPos[1], &rotYMatrixFaster ); //moon
	newPos[1] += newPos[0]; //translate back to planet.

	pMath->TVVec4Transform( &spyroPos, &spyroPos, &rotZMatrix ); //spyro
	pMath->TVVec4Transform( &newSpyroPos, &spyroPos, &rotXMatrix2 );
	newPos[2] = newSpyroPos;

	////////// end position ///////

	if( !ActorList.empty() )
	{		
		// see if you can get this to happen using your own matrix
		ActorList[0]->RotateY( -angle1 *100, false );

		ActorList[1]->SetPosition( newPos[0].x, newPos[0].y, newPos[0].z ); 
		ActorList[1]->SetRotationMatrix( &rotation[0] );

		ActorList[2]->SetPosition( newPos[1].x, newPos[1].y, newPos[1].z ); 
		ActorList[2]->SetRotationMatrix( &rotation[1] );

		ActorList[3]->SetPosition( newPos[2].x, newPos[2].y, newPos[2].z );
		ActorList[3]->SetRotationMatrix( &rotation[2] );
	}
	

	// text on the screen
	pText->Action_BeginText();

	std::ostringstream output;

	output<<"john rules!"<<std::endl;

	for (int i = 0; i < numActors; i++)
	{
		output<<"newx = "<<newPos[i].x<<" newy= "<<newPos[i].y<<" newz= "<<newPos[i].z<<std::endl;
	}
	
	pText->NormalFont_DrawText( output.str().c_str(), 10, 40, cTV_COLORKEY_GREEN );
	pText->Action_EndText();
}

void Object::AddActor( cTV_3DVECTOR position )
{
	CTVActor *pActor = pScene->CreateActor();
	pActor->LoadTVA( "Bruce_Run_Test8.TVA" );
	pActor->SetPosition( position.x, position.y, position.z);
	pActor->SetScale( 1.f, 1.f, 1.f);
	pActor->SetLightingMode(cTV_LIGHTING_NONE);
	pActor->SetAnimationByName("idle");
	pActor->PlayAnimation(1.f);

	ActorList.push_back( pActor );
}