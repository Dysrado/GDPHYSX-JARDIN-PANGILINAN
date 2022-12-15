#pragma once
#include "RigidBody.h"
class ForceGenerator
{
	virtual void updateForce(RigidBody* body, float duration) = 0;
};

