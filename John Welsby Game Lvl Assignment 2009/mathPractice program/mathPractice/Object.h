#pragma once

class Object
{
public:
	Object( CTVScene *pScene );
	~Object(void);

	void Update( float dt );

	void AddActor( cTV_3DVECTOR position );

private:
	CTVScene *pScene;
	CTVMathLibrary	*pMath;
	CTVScreen2DText *pText;
	
	std::vector<CTVActor*> ActorList;

	float			angle1, angle2, angle3;
	cTV_3DMATRIX	rotYMatrix, invRotYMatrix, rotYMatrixFaster, rotXMatrix, rotXMatrix2, rotZMatrix, rotation[3];
	cTV_4DVECTOR	spyroPos, newSpyroPos;

};
