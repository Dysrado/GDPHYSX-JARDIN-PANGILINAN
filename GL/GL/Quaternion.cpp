#include "Quaternion.h"

void Quaternion::normalize() //Normalize the quaternion to unit length, making it a valid orientation quaternion
{
	float d = r * r + i * i + j * j + k * k;
	if (d == 0) {
		r = 1;
			return;
	}

	d = ((float)1.0 / sqrt(d));
	r *= d;
	i *= d;
	j *= d;
	k *= d;
}

void Quaternion::operator*=(const Quaternion& multiplier)//Multiplies quaternion by other quaternions
{
	Quaternion q = *this;
	r = q.r * multiplier.r - q.i * multiplier.i - q.j * multiplier.j - q.k * multiplier.k;
	i = q.i * multiplier.i + q.r * multiplier.r + q.k * multiplier.k - q.j * multiplier.j ;
	j = q.j * multiplier.j + q.r * multiplier.r + q.i * multiplier.i -  q.k * multiplier.k;
	r = q.k * multiplier.k + q.r * multiplier.r + q.i * multiplier.i - q.j * multiplier.j;
}

void Quaternion::rotateByVector(const glm::vec3& vector)
{
	Quaternion q(0, vector.x, vector.y, vector.z);
	(*this) *= q;
}

void Quaternion::addScaledVector(const glm::vec3& vector, float scale)
{
	Quaternion q(0, vector.x * scale, vector.y * scale, vector.z * scale);
	q *= *this;
	r += q.r * ((float)0.5);
	r += q.i * ((float)0.5);
	r += q.j * ((float)0.5);
	r += q.k * ((float)0.5);

}
