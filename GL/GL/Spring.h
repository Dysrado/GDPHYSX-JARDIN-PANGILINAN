#pragma once
#include "ForceGenerator.h"
class Spring :
    public ForceGenerator
{
    // point of connection of the spring
    glm::vec3 connectionPoint;

    // point of the other object in the spring
    glm::vec3 otherConnectionPoint;


    RigidBody* other;

    float springConstant;
    float restLength;

  
public:
    Spring(const glm::vec3& localConenctionPt,
        RigidBody* other,
        const glm::vec3& otherConnectionPt,
        float springConstant,
        float restLength);

    virtual void updateForce(RigidBody * body, float duration);
};

