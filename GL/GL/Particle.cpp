#include "Particle.h"
#include <cmath>

void Particle::setActive(bool active)
{
	isActive = active;
}

float Particle::getMass()
{
	return this->inverseMass;
}

glm::vec3 Particle::getPosition()
{
	return position;
}

void Particle::setPosition(glm::vec3 pos)
{
	this->position = pos;
}


// Sets the position, scale, and rotation to the Particle class
void Particle::initVariables(glm::vec3 pos, glm::vec3 size, glm::vec3 rot, int type) {
	glm::mat4 identity = glm::mat4(1.f);
	// Sets the Particle's location, scale and rotation to the parameters location, scale and rotation
	switch (type) {
		case 1://When the particle selected is Artillery Ammo
			this->velocity = glm::vec3(0, 0, 35.0f);
			this->inverseMass = 1.0f;
			//this->acceleration = glm::vec3(0, -20.0f, 0);
			this->damping = 0.99f;
			break;	
		case 2:	
			this->velocity = glm::vec3(0, 0, 0);
			this->inverseMass = 0.9f;
			//this->acceleration = glm::vec3(0, -20.0f, 0);
			this->damping = 0.99f;
	}
	
	this->position = pos;
	this->rotation = rot;
	this->scale = size;
	transform = glm::translate(identity, this->position);
	transform = glm::scale(transform, this->scale);
	transform = glm::rotate(transform, glm::radians(this->rotation.x), glm::vec3(1, 0, 0));
	transform = glm::rotate(transform, glm::radians(this->rotation.y), glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, glm::radians(this->rotation.z), glm::vec3(0, 0, 1));
}

//Initializes the buffers
void Particle::init() {
	// initialitation for the Particle
	std::string path = "3D/box.obj";
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> material;
	std::string warning, error;

	tinyobj::attrib_t attributes;
	bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

	for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
		mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
	}

	// initialization of VAO, VBO, and EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(attributes.vertices) * attributes.vertices.size(), attributes.vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


//Applies physics to the particles over deltaTime
void Particle::integrate(float duration)
{
	if (duration > 0.0) { 
		position += velocity * duration; //Updates the position vector of the particle
		velocity += acceleration * duration; //Updates the velocity of the particle based on acceleration

		glm::vec3 resultAcc = acceleration;
		//Update forces
		
		resultAcc += forceAccum * inverseMass;
		velocity += resultAcc * duration;
		//velocity += acceleration * duration; //Updates the velocity of the particle based on acceleration
		velocity *= pow(damping, duration); //Applies damping to the velocity
		if (position.y <= -1.5f) {
			velocity.y = 0;
		}
		transform = glm::translate(glm::mat4(1.0f), position); //Appliess linear transformation to the particle
		transform = glm::scale(transform, this->scale);
		clearAccum();
	}
	
}



// Renders or draws the model
void Particle::render(GLuint shaderProgram) {
	if (isActive) {
		// set the location
		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// use the shader from the main code
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
	}
	
};

// Delete VAO, VBO, and EBO
void Particle::deleteVertex() {
	// delete the buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Particle::clearAccum()
{
	forceAccum = glm::vec3(0);
}

void Particle::addForce(const glm::vec3& force)
{
	forceAccum += force;
}

glm::vec3 Particle::getVelocity()
{
	return this->velocity;
}

void Particle::setVelocity(glm::vec3 vel)
{
	this->velocity = vel;
}

glm::vec3 Particle::getAcceleration()
{
	return acceleration;
}

glm::vec3 Particle::getForceAccum()
{
	return forceAccum;
}
