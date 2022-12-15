#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Quaternion
{
public:
	union {
		struct {
			float r;
			float i;
			float j;
			float k;


		};
			float data[4];

	};
	Quaternion(float a, float b, float c, float d);
	void normalize();
	void operator *= (const Quaternion& multiplier);
	void rotateByVector(const glm::vec3& vector);
	void addScaledVector(const glm::vec3& vector, float scale);
};

