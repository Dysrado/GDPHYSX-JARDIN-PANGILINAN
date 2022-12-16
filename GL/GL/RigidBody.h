#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "tiny_obj_loader.h"

#include "Quaternion.h"
#include "Matrix4.h"
#include "Matrix3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include "Particle.h"

class RigidBody
{
public:
	// constructor
	RigidBody(glm::vec3 pos, Quaternion rot);

	// mass
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

	Matrix3 inverseInertiaTensor;

	// transform matrix for making body space into world space
	Matrix4 transformMatrix;

	// accumulated forces
	glm::vec3 forceAccum;
	glm::vec3 torqueAccum;

	// accelerations
	glm::vec3 acceleration;
	glm::vec3 lastFrameAcceleration;

	

	//checks if the rigid body is still active
	bool isAwake;


	
	


	// for rendering
	GLuint VAO = 0, VBO = 0, EBO = 0;
	std::vector<GLuint> mesh_indices;

	// draws the rigid body
	void render(GLuint shaderProgram);

	void calculateDerivedData();


	// Laws of Motion for Rigid Bodies
	void setInertiaTensor(const Matrix3 &inertiaTensor);

	void addForce(const glm::vec3 &force);

	void addTorque(const glm::vec3& torque);
	
	void integrate(float duration);

	// removes all of the accumulated forces
	void clearAccumulators();

	glm::vec3 getPointInWorldSpace(const glm::vec3& point) const;

	void addForceAtBodyPoint(const glm::vec3 &force, const glm::vec3 &point);

	void addForceAtPoint(const glm::vec3& force, const glm::vec3& point);

	bool hasFiniteMass() const;

	float getMass() const;

	float angularDamping;
	float linearDamping;

	void setAcceleration(const float x, const float y, const float z);

	glm::vec3 getPosition();

};

