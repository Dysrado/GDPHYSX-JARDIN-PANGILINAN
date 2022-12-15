#include "RigidBody.h"

void RigidBody::calculateDerivedData()
{
    // calculates the transform matrix
    _calculateTransformMatrix(transformMatrix, position, orientation);
}



void RigidBody::setInertiaTensor(const Matrix3 &inertiaTensor)
{
    inverseInertiaTensor.setInverse(inertiaTensor);
    //_checkInverseInertiaTensor(inverseInertiaTensor);
}

static inline void _calculateTransformMatrix(Matrix4& transformMatrix,
    const glm::vec3& position,
    const Quaternion& orientation)
{
    transformMatrix.data[0] = 1 - 2 * orientation.j * orientation.j -
        2 * orientation.k * orientation.k;
    transformMatrix.data[1] = 2 * orientation.i * orientation.j -
        2 * orientation.r * orientation.k;
    transformMatrix.data[2] = 2 * orientation.i * orientation.k +
        2 * orientation.r * orientation.j;
    transformMatrix.data[3] = position.x;

    transformMatrix.data[4] = 2 * orientation.i * orientation.j +
        2 * orientation.r * orientation.k;
    transformMatrix.data[5] = 1 - 2 * orientation.i * orientation.i -
        2 * orientation.k * orientation.k;
    transformMatrix.data[6] = 2 * orientation.j * orientation.k -
        2 * orientation.r * orientation.i;
    transformMatrix.data[7] = position.y;

    transformMatrix.data[8] = 2 * orientation.i * orientation.k -
        2 * orientation.r * orientation.j;
    transformMatrix.data[9] = 2 * orientation.j * orientation.k +
        2 * orientation.r * orientation.i;
    transformMatrix.data[10] = 1 - 2 * orientation.i * orientation.i -
        2 * orientation.j * orientation.j;
    transformMatrix.data[11] = position.z;
}

void RigidBody::addForce(const glm::vec3& force) {
    forceAccum += force;
    isAwake = true;
}

void RigidBody::integrate(float duration) {
    // get linear acceleration from the force input
    lastFrameAcceleration = acceleration;
    lastFrameAcceleration += forceAccum * inverseMass;

    // get angular acceleration from torque inputs
    glm::vec3 angularAcceleration = inverseInertiaTensorWorld.transform(torqueAccum);

    // adjust the velocities
    velocity += lastFrameAcceleration * duration;

    rotation += angularAcceleration * duration;

    // drag
    // check linear damping
    velocity *= pow(linearDamping, duration);
    rotation *= pow(angularDamping, duration);

    calculateDerivedData();

    clearAccumulators();
}

void RigidBody::clearAccumulators() {
    forceAccum = glm::vec3(0, 0, 0);
    torqueAccum = glm::vec3(0, 0, 0);
}

void RigidBody::addTorque(const glm::vec3& torque)
{
    torqueAccum += torque;
    isAwake = true;
}

glm::vec3 RigidBody::getPointInWorldSpace(const glm::vec3& point) const
{
    return transformMatrix.transform(point);
}

void RigidBody::addForceAtBodyPoint(const glm::vec3& force, const glm::vec3& point) {
    glm::vec3 pt = getPointInWorldSpace(point);
    
}

void RigidBody::addForceAtPoint(const glm::vec3& force,
    const glm::vec3& point)
{
    // Convert to coordinates relative to center of mass.
    glm::vec3 pt = point;
    pt -= position;

    forceAccum += force;
    torqueAccum += pt % force;

    isAwake = true;
}

bool RigidBody::hasFiniteMass() const
{
    return inverseMass >= 0.0f;
}

float RigidBody::getMass() const
{
    if (inverseMass == 0) {
        return 10000;
    }
    else {
        return ((float)1.0) / inverseMass;
    }
}

void RigidBody::setAcceleration(const float x, const float y, const float z)
{
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}
