#include "RigidBody.h"
/// <summary>
/// Constructor
/// </summary>
/// <param name="pos"> position of the rigidbody </param>
/// <param name="rot"> orientation in quaternion of the rigidbody </param>
RigidBody::RigidBody(glm::vec3 pos, Quaternion rot)
{
    // damping
    linearDamping = .95f;
    angularDamping = .95f;

    // mass
    inverseMass = 1;

    // setting the rotation and position
    orientation = rot;
    position = pos;

    // placing rotation and position to the transform matrix
    transformMatrix.setOrientationAndPos(rot, pos);

    // intertia tensor initialization
    Matrix3 identity;
    identity.data[0] = 1 / 12 * inverseMass * (2);
    identity.data[4] = 1 / 12 * inverseMass * (2);
    identity.data[8] = 1 / 12 * inverseMass * (2);
    setInertiaTensor(identity);

    // initialitation for the rendering of the rigidbody
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

/// <summary>
/// Calclates the position and orientation of the rigidbody and places it into the transform matrix
/// </summary>
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
    //      created these variables for readability
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
}


/// <summary>
/// Sets the local InertiaTensor
/// </summary>
/// <param name="inertiaTensor"></param>
void RigidBody::setInertiaTensor(const Matrix3 &inertiaTensor)
{
    inverseInertiaTensor.setInverse(inertiaTensor);
}

/// <summary>
/// Adds force to the overall rigidbody
/// </summary>
/// <param name="force"> amount of force to apply </param>
void RigidBody::addForce(const glm::vec3& force) {
    forceAccum += force;
    isAwake = true;
}

/// <summary>
/// Calculations for the physics
/// </summary>
/// <param name="duration"> the time difference of each frame </param>
void RigidBody::integrate(float duration) {
    if (!isAwake) return;
    // get linear acceleration from the force input
    lastFrameAcceleration = acceleration;
    lastFrameAcceleration += forceAccum * inverseMass;

    // get angular acceleration from torque inputs
    glm::vec3 angularAcceleration = inverseInertiaTensorWorld.transform(torqueAccum);

    // adjust the velocities
    velocity += lastFrameAcceleration * duration;

    rotation += angularAcceleration * duration;
    
    // drag
    velocity *= pow(linearDamping, duration);
    rotation *= pow(angularDamping, duration);

    position += velocity * duration;
    orientation.addScaledVector(rotation, duration);

    // reset values
    calculateDerivedData();
    clearAccumulators();
    //isAwake = false;
}

/// <summary>
/// Removes the forces from the Rigidbody
/// </summary>
void RigidBody::clearAccumulators() {
    forceAccum = glm::vec3(0, 0, 0);
    torqueAccum = glm::vec3(0, 0, 0);
}

/// <summary>
/// Adds torque to the rigidbody
/// </summary>
/// <param name="torque"> how much torque you want to apply </param>
void RigidBody::addTorque(const glm::vec3& torque)
{
    torqueAccum += torque;
    isAwake = true;
}


/// <summary>
/// Renders the rigidbody
/// </summary>
/// <param name="shaderProgram"> shader data used to render the rigidbody </param>
void RigidBody::render(GLuint shaderProgram) {
    // Convert the transformMatrix into a glm::mat4
    // place the transform to the matrix
    glm::mat4 transform = glm::mat4(1.f);
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

/// <summary>
/// Gets a point of the rigidbody then converts it into the world space location
/// </summary>
/// <param name="point"> point to convert </param>
/// <returns> converted point into world space </returns>
glm::vec3 RigidBody::getPointInWorldSpace(const glm::vec3& point) const
{
    return transformMatrix.transform(point);
}

/// <summary>
/// Adds force into a point of the rigidbody
/// </summary>
/// <param name="force"> how much force should be applied to the body </param>
/// <param name="point"> where should the force be applied </param>
void RigidBody::addForceAtBodyPoint(const glm::vec3& force, const glm::vec3& point) {
    glm::vec3 pt = getPointInWorldSpace(point);
    
    addForceAtPoint(force, pt); 
    isAwake = true;
    
}

/// <summary>
/// Adds force into a point
/// </summary>
/// <param name="force"> how much force should be applied to the body </param>
/// <param name="point"> where should the force be applied </param>
void RigidBody::addForceAtPoint(const glm::vec3& force,
    const glm::vec3& point)
{
    // Convert to coordinates relative to center of mass.
    glm::vec3 pt = point;
    pt -= position;

    forceAccum += force;
    torqueAccum += glm::mod(pt, force);

    isAwake = true;
}

/// <summary>
/// Checks if the rigidbody is movable
/// </summary>
/// <returns> a bool of if it is movable </returns>
bool RigidBody::hasFiniteMass() const
{
    return inverseMass >= 0.0f;
}

/// <summary>
/// Gets the mass of the rigidbody
/// </summary>
/// <returns> either the mass of the rigidbody or mass of an imovable object </returns>
float RigidBody::getMass() const
{
    if (inverseMass == 0) {
        return 10000;
    }
    else {
        return ((float)1.0) / inverseMass;
    }
}

/// <summary>
/// sets the acceleration
/// </summary>
/// <param name="x"> acceleration in the x-axis </param>
/// <param name="y"> acceleration in the y-axis </param>
/// <param name="z"> acceleration in the z-axis </param>
void RigidBody::setAcceleration(const float x, const float y, const float z)
{
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}

/// <summary>
/// Gets the position of the rigidbody
/// </summary>
/// <returns> the position of the rigidbody </returns>
glm::vec3 RigidBody::getPosition()
{
    glm::vec3 temp = glm::vec3(transformMatrix.data[3],
        transformMatrix.data[7],
        transformMatrix.data[11]);
    return temp;
}
