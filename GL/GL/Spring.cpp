#include "Spring.h"
#include <cmath>

Spring::Spring(const glm::vec3& localConenctionPt, RigidBody* other, const glm::vec3& otherConnectionPt, float springConstant, float restLength)
{
	connectionPoint = localConenctionPt;
	otherConnectionPoint = otherConnectionPt;

	this->other = other;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void Spring::updateForce(RigidBody* body, float duration)
{
	// calculate the two ends in world space
	glm::vec3 lws = body->getPointInWorldSpace(connectionPoint);
	glm::vec3 ows = other->getPointInWorldSpace(otherConnectionPoint);

	glm::vec3 force = lws - ows;

	// calculate the magniture of the force
	float magnitude = glm::length(force);
	magnitude = abs(magnitude - restLength);
	magnitude *= springConstant;

	force = glm::normalize(force);
	force *= -magnitude;
	body->addForceAtPoint(force, lws);
}
