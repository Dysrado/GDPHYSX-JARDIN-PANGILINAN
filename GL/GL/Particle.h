#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <cmath>
#include <iostream>
#include <vector>

class Particle
{
protected:
	glm::vec3 position = glm::vec3(0);
	float inverseMass = 0.f;
private:
	GLuint VAO = 0, VBO = 0, EBO = 0;
	std::vector<GLuint> mesh_indices;
	glm::vec3 scale = glm::vec3(1);
	glm::vec3 rotation = glm::vec3(0);
	glm::mat4 transform = glm::mat4(1.f);



	glm::vec3 velocity = glm::vec3(0);
	glm::vec3 acceleration;
	float gravity = 15.0f;
	float force = 0.1f;

	glm::vec3 forceAccum;
	
	float damping = 0.f;

public:
	float getMass();
	void initVariables(glm::vec3 pos, glm::vec3 size, glm::vec3 rot, int type);
	void init();
	void integrate(float duration);
	void render(GLuint shaderProgram);
	void deleteVertex();
	void clearAccum();
	void addForce(const glm::vec3& force);

};

