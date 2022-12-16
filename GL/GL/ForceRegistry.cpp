#include "ForceRegistry.h"

void ForceRegistry::add(RigidBody* rigidbody, ForceGenerator* fg)
{
	ForceRegistration temp;
	temp.rigidbody = rigidbody;
	temp.fg = fg;
	registrations.push_back(temp);
}

void ForceRegistry::remove(RigidBody* rigidbody, ForceGenerator* fg)
{
}

void ForceRegistry::clear()
{
	registrations.clear();
}

void ForceRegistry::updateForces(float duration)
{
	for (int i = 0; i < registrations.size(); i++) {
		registrations[i].fg->updateForce(registrations[i].rigidbody, duration);
	}
}
