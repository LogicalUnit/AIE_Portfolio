#ifndef ET_BALL_H
#define ET_BALL_H

#include "Common.h"
#include "Sphere.h"

extern Vertex gravity;

/* Ball class */
class Ball: public Sphere
{
public:

	Vertex velocity, force;
	float mass, oom, bounce;

	/* Default Constructor */
	Ball();

	/* Default Destructr */
	~Ball();

	/* Physics update function */
	void Update();

	inline void SetMass(float mass_)
	{
		mass = mass_;
		oom = 1.0f / mass;
	}

	inline void SetBounce(float bounce_)
	{
		bounce = bounce_;
	}

	inline void SetVelocity(const Vertex &velocity_)
	{
		velocity = velocity_;
		force = velocity_;
	}

	inline void SetForce(const Vertex &force_)
	{
		force = force_;
	}

private:

protected:
};
#endif