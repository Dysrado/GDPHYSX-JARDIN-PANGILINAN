#include "Matrix3.h"

glm::vec3 Matrix3::operator*(const glm::vec3& vector) const
{
	return glm::vec3(
		vector.x * data[0] + vector.y * data[1] + vector.z * data[2],
		vector.x * data[3] + vector.y * data[4] + vector.z * data[5],
		vector.x * data[6] + vector.y * data[7] + vector.z * data[8]);
}

glm::vec3 Matrix3::transform(const glm::vec3& vector) const
{
	return (*this) * vector;
}

Matrix3 Matrix3::operator*(const Matrix3& o) const
{
	 Matrix3 mat3;
	 mat3.data[0] = data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6];
	 mat3.data[1] = data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7];
	 mat3.data[2] = data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8];
	 mat3.data[3] = data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6];
	 mat3.data[4] = data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7];
	 mat3.data[5] = data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8];
	 mat3.data[6] = data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6];
	 mat3.data[7] = data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7];
	 mat3.data[8] = data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8];
	 return mat3;
}

void Matrix3::operator*=(const Matrix3& o)
{
	float t1; 
	float t2;
	float t3;
	t1 = data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6]; 
	t2 = data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7]; 
	t3 = data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8];
	data[0] = t1; data[1] = t2; 
	data[2] = t3; 
	t1 = data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6]; 
	t2 = data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7]; 
	t3 = data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8]; 
	data[3] = t1; data[4] = t2;
	data[5] = t3; 
	t1 = data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6];
	t2 = data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7];
	t3 = data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8]; 
	data[6] = t1; data[7] = t2; data[8] = t3;
}

void Matrix3::setInverse(const Matrix3& m)
{
	float t1 = m.data[0] * m.data[4]; 
	float t2 = m.data[0] * m.data[5];
	float t3 = m.data[1] * m.data[3];
	float t4 = m.data[2] * m.data[3];
	float t5 = m.data[1] * m.data[6];
	float t6 = m.data[2] * m.data[6];
	
	// Calculate the determinant.
	float det = (t1*m.data[8] - t2*m.data[7] - t3*m.data[8]+t4*m.data[7] + t5*m.data[5] - t6*m.data[4]);
	
	// Make sure the determinant is non-zero.
	if (det == (float)0.0f)
		return;
	float invd = (float)1.0f/det;
	data[0] = (m.data[4]*m.data[8]-m.data[5]*m.data[7])*invd;
	data[1] = -(m.data[1]*m.data[8]-m.data[2]*m.data[7])*invd;
	data[2] = (m.data[1]*m.data[5]-m.data[2]*m.data[4])*invd;
	data[3] = -(m.data[3]*m.data[8]-m.data[5]*m.data[6])*invd;
	data[4] = (m.data[0]*m.data[8]-t6)*invd;
	data[5] = -(t2-t4)*invd;
	data[6] = (m.data[3]*m.data[7]-m.data[4]*m.data[6])*invd;
	data[7] = -(m.data[0]*m.data[7]-t5)*invd;
	data[8] = (t1-t3)*invd;
}

Matrix3 Matrix3::inverse() const
{
	Matrix3 result;
	result.setInverse(*this); 
	return result;
}

void Matrix3::invert()
{
	setInverse(*this);
}

void Matrix3::setTranspose(const Matrix3& m)
{
	data[0] = m.data[0]; 
	data[1] = m.data[3]; 
	data[2] = m.data[6]; 
	data[3] = m.data[1]; 
	data[4] = m.data[4]; 
	data[5] = m.data[7]; 
	data[6] = m.data[2]; 
	data[7] = m.data[5]; 
	data[8] = m.data[8];
}



Matrix3 Matrix3::transpose() const
{
	Matrix3 result;
	result.setTranspose(*this);
	return result;
}

void Matrix3::setOrientation(const Quaternion& q)
{
	data[0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k); 
	data[1] = 2 * q.i * q.j + 2 * q.k * q.r; 
	data[2] = 2 * q.i * q.k - 2 * q.j * q.r; 
	data[3] = 2 * q.i * q.j - 2 * q.k * q.r; 
	data[4] = 1 - (2 * q.i * q.i + 2 * q.k * q.k); 
	data[5] = 2 * q.j * q.k + 2 * q.i * q.r; 
	data[6] = 2 * q.i * q.k + 2 * q.j * q.r; 
	data[7] = 2 * q.j * q.k - 2 * q.i * q.r; 
	data[8] = 1 - (2 * q.i * q.i + 2 * q.j * q.j);
}

