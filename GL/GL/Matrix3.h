#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Quaternion.h"
class Matrix3
{
	float data[9];
	
	glm::vec3 operator*(const glm::vec3& vector) const;
	glm::vec3 transform(const glm::vec3& vector) const;
	Matrix3 operator*(const Matrix3& o) const;
	void operator*=(const Matrix3& o);
	void setInverse(const Matrix3& m);
	Matrix3 inverse() const;
	void invert();
	void setTranspose(const Matrix3& m);
	Matrix3 transpose() const;
	void setOrientation(const Quaternion& q);
};
