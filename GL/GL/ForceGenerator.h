#pragma once
#include "RigidBody.h"
class ForceGenerator
{
public:
	virtual void updateForce(RigidBody* body, float duration) = 0;
};

