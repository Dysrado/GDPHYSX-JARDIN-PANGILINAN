#pragma once
#include "ForceGenerator.h"
class Gravity :
	public ForceGenerator {
	/** Holds the acceleration due to gravity. */
	glm::vec3 gravity;
public:

	/** Creates the generator with the given acceleration. */
	Gravity(const glm::vec3& gravity);

	/** Applies the gravitational force to the given rigid body. */
	virtual void updateForce(RigidBody* body, float duration);
};

