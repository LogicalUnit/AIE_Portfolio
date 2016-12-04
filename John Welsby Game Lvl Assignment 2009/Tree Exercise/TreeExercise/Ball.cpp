#include "stdafx.h"
#include "Ball.h"

Ball::Ball()
{
}

Ball::~Ball()
{
}

void Ball::Update()
{
	force = gravity / oom;

	velocity += delta * oom * force;

	origin += delta * velocity;
}