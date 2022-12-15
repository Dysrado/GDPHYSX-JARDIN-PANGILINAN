#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include "tiny_obj_loader.h"

#include "Quaternion.h"
#include "Matrix4.h"
#include "Matrix3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RigidBody
{
public:
	float inverseMass;

	// position of the RigidBody in world space
	glm::vec3 position;

	// angular orientation of the RigidBody in world space
	Quaternion orientation;
	//glm::quat orientation;

	// linear velocity of the rigid body in world space
	glm::vec3 velocity;

	// angular velocity of the rigid body in world space
	glm::vec3 rotation;
	
	// inverse inertia tensor of the body in world space
	Matrix3 inverseInertiaTensorWorld;

	// transform matrix for making body space into world space
	Matrix4 transformMatrix;

	glm::vec3 forceAccum;
	glm::vec3 torqueAccum;

	glm::vec3 acceleration;
	glm::vec3 lastFrameAcceleration;

	Matrix3 inverseInertiaTensor;

	bool isAwake;

	void calculateDerivedData();

	// Laws of Motion for Rigid Bodies
	void setInertiaTensor(const Matrix3 &inertiaTensor);

	void addForce(const glm::vec3 &force);

	void addTorque(const glm::vec3& torque);
	
	void integrate(float duration);

	void clearAccumulators();

	glm::vec3 getPointInWorldSpace(const glm::vec3& point) const;

	void addForceAtBodyPoint(const glm::vec3 &force, const glm::vec3 &point);

	void addForceAtPoint(const glm::vec3& force, const glm::vec3& point);

	bool hasFiniteMass() const;

	float getMass() const;

	float angularDamping;
	float linearDamping;

	void setAcceleration(const float x, const float y, const float z);

};

