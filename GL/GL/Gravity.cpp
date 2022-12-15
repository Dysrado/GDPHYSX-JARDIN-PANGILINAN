#include "Gravity.h"

void Gravity::updateForce(RigidBody* body, float duration)
{
	if (!body->hasFiniteMass()) return;
	body->addForce(gravity * body->getMass());
}
