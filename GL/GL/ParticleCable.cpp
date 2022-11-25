#include "ParticleCable.h"

float ParticleLink::currentLength() const {
    glm::vec3 relativePos = particle[0]->getPosition() - particle[1]->getPosition();
    return relativePos.length();
}

unsigned ParticleCable::fillContact(ParticleContact* contact, unsigned limit) const
{
    float length = currentLength();

    if (length < maxLength) {
        return 0;
    }

    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    glm::vec3 normal = particle[1]->getPosition() - particle[0]->getPosition();
    normal = glm::normalize(normal);
    contact->contactNormal = normal;

    contact->penetration = length - maxLength;
    contact->restitution = restitution;

    return 1;
}
