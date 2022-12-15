#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Quaternion.h"
class Matrix4
{
public:
	//Holds the matrix data in array form
	float data[12];

	glm::vec3 operator*(const glm::vec3& vector) const;
	glm::vec3 transform(const glm::vec3& vector) const;
	Matrix4 operator* (const Matrix4& o) const;
	float getDeterminant() const;
	void setInverse(const Matrix4& m);
	Matrix4 inverse() const;
	void invert();

	void setOrientationAndPos(const Quaternion& q, const glm::vec3& pos);
	glm::vec3 transformInverse(const glm::vec3& vector) const;
	glm::vec3 transformDirection(const glm::vec3& vector) const;
	glm::vec3 transformInverseDirection(const glm::vec3& vector) const;
	glm::vec3 localToWorldDirn(const glm::vec3& local, const Matrix4& transform);
	
	glm::vec3 worldToLocalDirn(const glm::vec3& world, const Matrix4& transform);

};

