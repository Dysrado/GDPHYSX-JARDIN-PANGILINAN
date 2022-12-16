#include "RigidBody.h"

RigidBody::RigidBody(glm::vec3 pos, Quaternion rot)
{
    inverseMass = 1;
    orientation = rot;
    position = pos;
    transformMatrix.setOrientationAndPos(rot, pos);

    // initialitation for the Particle
    std::string path = "3D/box.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;
    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    // initialization of VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(attributes.vertices) * attributes.vertices.size(), attributes.vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RigidBody::calculateDerivedData()
{
    // calculates the transform matrix
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



void RigidBody::setInertiaTensor(const Matrix3 &inertiaTensor)
{
    inverseInertiaTensor.setInverse(inertiaTensor);
    //_checkInverseInertiaTensor(inverseInertiaTensor);
}

//static inline void _calculateTransformMatrix(Matrix4& transformMatrix,
//    const glm::vec3& position,
//    const Quaternion& orientation)
//{
//    
//}

void RigidBody::addForce(const glm::vec3& force) {
    forceAccum += force;
    isAwake = true;
}

void RigidBody::integrate(float duration) {
    if (!isAwake) return;
    // get linear acceleration from the force input
    lastFrameAcceleration = acceleration;
    lastFrameAcceleration += forceAccum * inverseMass;
    std::cout << "Force Acumm A: " << forceAccum.x << " " << forceAccum.y << " " << forceAccum.z << std::endl;

    // get angular acceleration from torque inputs
    glm::vec3 angularAcceleration = inverseInertiaTensorWorld.transform(torqueAccum);

    std::cout << "Torque Acumm A: " << torqueAccum.x << " " << torqueAccum.y << " " << torqueAccum.z << std::endl;
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



void RigidBody::render(GLuint shaderProgram) {
    // Convert the transformMatrix into a glm::mat4
    // place the transform to the matrix
    glm::mat4 transform = glm::mat4(1.f);
    //transform = glm::scale(transform, glm::vec3(4,4,4));
    transform = glm::translate(transform, 
        glm::vec3(
            transformMatrix.data[3],
            transformMatrix.data[7],
            transformMatrix.data[11]
        ));

    // place the quaternion to the matrix
    glm::quat quaternion = glm::quat(
        orientation.r,
        orientation.i,
        orientation.j,
        orientation.k
    );
    quaternion[0] = orientation.data[0];
    quaternion[1] = orientation.data[1];
    quaternion[2] = orientation.data[2];
    quaternion[3] = orientation.data[3];
    transform *= glm::toMat4(quaternion);

    // set the location
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    // use the shader from the main code
    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
};

glm::vec3 RigidBody::getPointInWorldSpace(const glm::vec3& point) const
{
    return transformMatrix.transform(point);
}

void RigidBody::addForceAtBodyPoint(const glm::vec3& force, const glm::vec3& point) {
    glm::vec3 pt = getPointInWorldSpace(point);
    addForceAtPoint(force, pt); 
    isAwake = true;
    
}

void RigidBody::addForceAtPoint(const glm::vec3& force,
    const glm::vec3& point)
{

    // Convert to coordinates relative to center of mass.
    glm::vec3 pt = point;
    pt -= position;
    std::cout << "Force Acumm A: " << forceAccum.x << " " << forceAccum.y << " " << forceAccum.z << std::endl;
    std::cout << "Force Acumm A: " << torqueAccum.x << " " << torqueAccum.y << " " << torqueAccum.z << std::endl;
    forceAccum += force;

    torqueAccum += glm::mod(pt, force);
    std::cout << "Force Acumm B: " << forceAccum.x << " " << forceAccum.y << " " << forceAccum.z << std::endl;
    std::cout << "Force Acumm B: " << torqueAccum.x << " " << torqueAccum.y << " " << torqueAccum.z << std::endl;


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
