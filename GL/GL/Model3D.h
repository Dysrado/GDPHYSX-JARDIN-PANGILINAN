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

class Model3D
{
private:
	GLuint VAO = 0, VBO = 0, EBO = 0;
	std::vector<GLuint> mesh_indices;
	glm::vec3 position = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);
	glm::vec3 rotation = glm::vec3(0);
	glm::mat4 transform = glm::mat4(1.f);

public:
	void initVariables(glm::vec3 pos, glm::vec3 size, glm::vec3 rot);
	void init();
	
	void render(GLuint shaderProgram);
	void deleteVertex();
};

