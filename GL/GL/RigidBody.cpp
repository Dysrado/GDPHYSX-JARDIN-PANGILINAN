#include "RigidBody.h"

RigidBody::RigidBody(glm::vec3 pos, Quaternion rot)
{
    linearDamping = .95f;
    angularDamping = .95f;
    inverseMass = 1;
    orientation = rot;
    position = pos;
    transformMatrix.setOrientationAndPos(rot, pos);


    Matrix3 identity;
    identity.data[0] = 1 / 12 * inverseMass * (2);
    identity.data[4] = 1 / 12 * inverseMass * (2);
    identity.data[8] = 1 / 12 * inverseMass * (2);
    setInertiaTensor(identity);

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

    // calculates the inverseInertiaTensorWorld
    Matrix4 rotmat = transformMatrix;
    Matrix3 iitBody = inverseInertiaTensor;
    Matrix3 iitWorld = inverseInertiaTensorWorld;

    float t4 = rotmat.data[0] * iitBody.data[0] + rotmat.data[1] * iitBody.data[3] + rotmat.data[2] * iitBody.data[6]; 
    float t9 = rotmat.data[0] * iitBody.data[1] + rotmat.data[1] * iitBody.data[4] + rotmat.data[2] * iitBody.data[7];
    float t14 = rotmat.data[0] * iitBody.data[2] + rotmat.data[1] * iitBody.data[5] + rotmat.data[2] * iitBody.data[8];
    float t28 = rotmat.data[4] * iitBody.data[0] + rotmat.data[5] * iitBody.data[3] + rotmat.data[6] * iitBody.data[6];
    float t33 = rotmat.data[4] * iitBody.data[1] + rotmat.data[5] * iitBody.data[4] + rotmat.data[6] * iitBody.data[7];
    float t38 = rotmat.data[4] * iitBody.data[2] + rotmat.data[5] * iitBody.data[5] + rotmat.data[6] * iitBody.data[8];
    float t52 = rotmat.data[8] * iitBody.data[0] + rotmat.data[9] * iitBody.data[3] + rotmat.data[10] * iitBody.data[6];
    float t57 = rotmat.data[8] * iitBody.data[1] + rotmat.data[9] * iitBody.data[4] + rotmat.data[10] * iitBody.data[7];
    float t62 = rotmat.data[8] * iitBody.data[2] + rotmat.data[9] * iitBody.data[5] + rotmat.data[10] * iitBody.data[8];

    inverseInertiaTensorWorld.data[0] = t4 * rotmat.data[0] + t9 * rotmat.data[1] + t14 * rotmat.data[2];
    inverseInertiaTensorWorld.data[1] = t4 * rotmat.data[4] + t9 * rotmat.data[5] + t14 * rotmat.data[6]; 
    inverseInertiaTensorWorld.data[2] = t4 * rotmat.data[8] + t9 * rotmat.data[9] + t14 * rotmat.data[10];
    inverseInertiaTensorWorld.data[3] = t28 * rotmat.data[0] + t33 * rotmat.data[1] + t38 * rotmat.data[2];
    inverseInertiaTensorWorld.data[4] = t28 * rotmat.data[4] + t33 * rotmat.data[5] + t38 * rotmat.data[6];
    inverseInertiaTensorWorld.data[5] = t28 * rotmat.data[8] + t33 * rotmat.data[9] + t38 * rotmat.data[10];
    inverseInertiaTensorWorld.data[6] = t52 * rotmat.data[0] + t57 * rotmat.data[1] + t62 * rotmat.data[2];
    inverseInertiaTensorWorld.data[7] = t52 * rotmat.data[4] + t57 * rotmat.data[5] + t62 * rotmat.data[6];
    inverseInertiaTensorWorld.data[8] = t52 * rotmat.data[8] + t57 * rotmat.data[9] + t62 * rotmat.data[10];

    transformMatrix = rotmat;
    inverseInertiaTensor = iitBody;
    //inverseInertiaTensorWorld = iitWorld; 
}



void RigidBody::setInertiaTensor(const Matrix3 &inertiaTensor)
{
    inverseInertiaTensor.setInverse(inertiaTensor);
    //_checkInverseInertiaTensor(inverseInertiaTensor);
}


void RigidBody::addForce(const glm::vec3& force) {
    forceAccum += force;
    isAwake = true;
}

void RigidBody::integrate(float duration) {
    if (!isAwake) return;
    // get linear acceleration from the force input
    lastFrameAcceleration = acceleration;
    lastFrameAcceleration += forceAccum * inverseMass;

    std::cout << "acceleration: " << acceleration.x << " " << acceleration.y << " " << acceleration.z << std::endl;
    //std::cout << "Position: " << getPosition().z << "\n";

    // get angular acceleration from torque inputs
    glm::vec3 angularAcceleration = inverseInertiaTensorWorld.transform(torqueAccum);

    //std::cout << "angularAcceleration: " << angularAcceleration.x << " " << angularAcceleration.y << " " << angularAcceleration.z << std::endl;
    
    // adjust the velocities
    velocity += lastFrameAcceleration * duration;

    rotation += angularAcceleration * duration;

    // drag
    // check linear damping
    velocity *= pow(linearDamping, duration);
    rotation *= pow(angularDamping, duration);

    position += velocity * duration;
    std::cout << "rotation: " << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;
    orientation.addScaledVector(rotation, duration);

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
    
    //std::cout << "pt " << pt.x << " " << pt.y << " " << pt.z << "\n"; // UNDEFINED
    addForceAtPoint(force, pt); 
    isAwake = true;
    
}

void RigidBody::addForceAtPoint(const glm::vec3& force,
    const glm::vec3& point)
{

    // Convert to coordinates relative to center of mass.
    glm::vec3 pt = point;
    pt -= position;

    forceAccum += force;
    torqueAccum += glm::mod(pt, force);

    //std::cout << "Force Acumm: " << forceAccum.x << " " << forceAccum.y << " " << forceAccum.z << std::endl;

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

glm::vec3 RigidBody::getPosition()
{
    glm::vec3 temp = glm::vec3(transformMatrix.data[3],
        transformMatrix.data[7],
        transformMatrix.data[11]);
    return temp;
}
