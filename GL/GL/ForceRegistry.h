#pragma once
#include "RigidBody.h"
#include "ForceGenerator.h"
class ForceRegistry
{
protected:
	struct ForceRegistration {
		RigidBody* rigidbody;
		ForceGenerator* fg;
	}; // A structure that contains the particle and the associated force generator

	typedef std::vector<ForceRegistration> Registry;
	Registry registrations; // List of Registrations

public:
	void add(RigidBody* rigidbody, ForceGenerator* fg); // adds the particle and force generator to the list
	void remove(RigidBody* rigidbody, ForceGenerator* fg);

	void clear(); // removes all of the data from registrations

	void updateForces(float duration); // updates all of the particles with associated forces
};

