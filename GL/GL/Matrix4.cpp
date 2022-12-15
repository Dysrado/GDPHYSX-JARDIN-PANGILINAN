#include "Matrix4.h"

glm::vec3 Matrix4::operator*(const glm::vec3& vector) const //Transform given vector by this matrix
{
    return glm::vec3(
        vector.x * data[0] +
        vector.y * data[1] + 
        vector.z * data[2] + data[3],
        
        vector.x * data[4] + 
        vector.y * data[5] + 
        vector.z * data[6] + data[7],

        vector.x * data[8] +
        vector.y * data[9] +
        vector.z * data[10] + data[11]);
}
 
glm::vec3 Matrix4::transform(const glm::vec3& vector) const
{
    return (*this) * vector;
}

Matrix4 Matrix4::operator*(const Matrix4& o) const //Multiplies matrix with another matrix
{
    Matrix4 result; 
    result.data[0] = o.data[0] * data[0] + o.data[4] * data[1] + o.data[8] * data[2]; 
    result.data[4] = o.data[0] * data[4] + o.data[4] * data[5] + o.data[8] * data[6]; 
    result.data[8] = o.data[0] * data[8] + o.data[4] * data[9] + o.data[8] * data[10]; 
    result.data[1] = o.data[1] * data[0] + o.data[5] * data[1] + o.data[9] * data[2]; 
    result.data[5] = o.data[1] * data[4] + o.data[5] * data[5] + o.data[9] * data[6]; 
    result.data[9] = o.data[1] * data[8] + o.data[5] * data[9] + o.data[9] * data[10]; 
    result.data[2] = o.data[2] * data[0] + o.data[6] * data[1] + o.data[10] * data[2]; 
    result.data[6] = o.data[2] * data[4] + o.data[6] * data[5] + o.data[10] * data[6];
    result.data[10] = o.data[2] * data[8] + o.data[6] * data[9] + o.data[10] * data[10];
    result.data[3] = o.data[3] * data[0] + o.data[7] * data[1] + o.data[11] * data[2] + data[3]; 
    result.data[7] = o.data[3] * data[4] + o.data[7] * data[5] + o.data[11] * data[6] + data[7]; 
    result.data[11] = o.data[3] * data[8] + o.data[7] * data[9] + o.data[11] * data[10] + data[11]; 
    return result;
}

float Matrix4::getDeterminant() const
{
    return data[8] * data[5] * data[2]
        + data[4] * data[9] * data[2]
        + data[8] * data[1] * data[6]
        - data[0] * data[9] * data[6]
        - data[4] * data[1] * data[10]
        + data[0] * data[5] * data[10];
}

void Matrix4::setInverse(const Matrix4& m)
{
    float det = getDeterminant(); 
    if (det == 0) 
        return; 
    det = ((float)1.0f) / det; 
    data[0] = (-m.data[9] * m.data[6] + m.data[5] * m.data[10]) * det; 
    data[4] = (m.data[8] * m.data[6] - m.data[4] * m.data[10]) * det; 
    data[8] = (-m.data[8] * m.data[5] + m.data[4] * m.data[9] * m.data[15]) * det; 
    data[1] = (m.data[9] * m.data[2] - m.data[1] * m.data[10]) * det; 
    data[5] = (-m.data[8] * m.data[2] + m.data[0] * m.data[10]) * det; 
    data[9] = (m.data[8] * m.data[1] - m.data[0] * m.data[9] * m.data[15]) * det; 
    data[2] = (-m.data[5] * m.data[2] + m.data[1] * m.data[6] * m.data[15]) * det;
    data[6] = (+m.data[4] * m.data[2] - m.data[0] * m.data[6] * m.data[15]) * det; 
    data[10] = (-m.data[4] * m.data[1] + m.data[0] * m.data[5] * m.data[15]) * det; 
    data[3] = (m.data[9] * m.data[6] * m.data[3] 
        - m.data[5] * m.data[10] * m.data[3]
        - m.data[9] * m.data[2] * m.data[7]
        + m.data[1] * m.data[10] * m.data[7]
        + m.data[5] * m.data[2] * m.data[11]
        - m.data[1] * m.data[6] * m.data[11]) * det; 
    data[7] = (-m.data[8] * m.data[6] * m.data[3]
        + m.data[4] * m.data[10] * m.data[3]
        + m.data[8] * m.data[2] * m.data[7]
        - m.data[0] * m.data[10] * m.data[7]
        - m.data[4] * m.data[2] * m.data[11]
        + m.data[0] * m.data[6] * m.data[11]) * det; 
    data[11] = (m.data[8] * m.data[5] * m.data[3]
        - m.data[4] * m.data[9] * m.data[3]
        - m.data[8] * m.data[1] * m.data[7]
        + m.data[0] * m.data[9] * m.data[7]
        + m.data[4] * m.data[1] * m.data[11]
        - m.data[0] * m.data[5] * m.data[11]) * det;
}

Matrix4 Matrix4::inverse() const
{
    Matrix4 result;
    result.setInverse(*this);
    return result;
}

void Matrix4::invert()
{
    setInverse(*this);
}

void Matrix4::setOrientationAndPos(const Quaternion& q, const glm::vec3& pos)
{
    data[0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k); 
    data[1] = 2 * q.i * q.j + 2 * q.k * q.r; 
    data[2] = 2 * q.i * q.k - 2 * q.j * q.r; 
    data[3] = pos.x; data[4] = 2 * q.i * q.j - 2 * q.k * q.r; 
    data[5] = 1 - (2 * q.i * q.i + 2 * q.k * q.k); 
    data[6] = 2 * q.j * q.k + 2 * q.i * q.r; data[7] = pos.y; 
    data[8] = 2 * q.i * q.k + 2 * q.j * q.r;
    data[9] = 2 * q.j * q.k - 2 * q.i * q.r; 
    data[10] = 1 - (2 * q.i * q.i + 2 * q.j * q.j); 
    data[11] = pos.z;
}

glm::vec3 Matrix4::transformInverse(const glm::vec3& vector) const
{
    glm::vec3 tmp = vector; 
    tmp.x -= data[3]; 
    tmp.y -= data[7]; 
    tmp.z -= data[11]; 
    return glm::vec3(tmp.x * data[0] +
        tmp.y * data[4] +
        tmp.z * data[8],

        tmp.x * data[1] +
        tmp.y * data[5] +
        tmp.z * data[9],

        tmp.x * data[2] +
        tmp.y * data[6] +
        tmp.z * data[10]);
}

glm::vec3 Matrix4::transformDirection(const glm::vec3& vector) const
{
    return glm::vec3(vector.x * data[0] + 
        vector.y * data[1] + vector.z * data[2], 
        vector.x * data[4] + vector.y * data[5] + 
        vector.z * data[6], vector.x * data[8] +
        vector.y * data[9] + vector.z * data[10]);
}

glm::vec3 Matrix4::transformInverseDirection(const glm::vec3& vector) const
{
    return glm::vec3(vector.x * data[0] +
        vector.y * data[4] + 
        vector.z * data[8], 
        vector.x * data[1] + 
        vector.y * data[5] + vector.z * 
        data[9], vector.x * data[2] + 
        vector.y * data[6] + vector.z * data[10]);
}

glm::vec3 Matrix4::localToWorldDirn(const glm::vec3& local, const Matrix4& transform)
{
    return transform.transformDirection(local);
}

glm::vec3 Matrix4::worldToLocalDirn(const glm::vec3& world, const Matrix4& transform)
{
    return transform.transformInverseDirection(world);
}
