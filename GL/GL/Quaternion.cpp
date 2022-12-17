#include "Quaternion.h"

Quaternion::Quaternion(float a, float b, float c, float d)
{
	r = a;
	i = b;
	j = c;
	k = d;
}

Quaternion::Quaternion()
{
}

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
	i = q.r * multiplier.i + q.i * multiplier.r + q.j * multiplier.k - q.k * multiplier.j;
	j = q.r * multiplier.j + q.j * multiplier.r + q.k * multiplier.i - q.i * multiplier.k;
	k = q.r * multiplier.k + q.k * multiplier.r + q.i * multiplier.j - q.j * multiplier.i;
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
	i += q.i * ((float)0.5);
	j += q.j * ((float)0.5);
	k += q.k * ((float)0.5);

}
