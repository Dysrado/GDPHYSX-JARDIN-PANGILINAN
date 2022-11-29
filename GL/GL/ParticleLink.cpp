#include "ParticleLink.h"

float ParticleLink::currentLength() const {
	glm::vec3 relativePos = particle[0]->getPosition() - particle[1]->getPosition();
	return sqrt((relativePos.x * relativePos.x) + (relativePos.y * relativePos.y) + (relativePos.z * relativePos.z));
	
}
unsigned ParticleRod::addContact(ParticleContact* contact, unsigned limit) const
{
    // Find the length of the rod
    float currentLen = currentLength();

    // Check if it over-extends
    if (currentLen == length) {
        return 0;
    }

    // Otherwise return the contact
    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    // Calculate the normal
    glm::vec3 normal = particle[1]->getPosition() - particle[0]->getPosition();
    normal = glm::normalize(normal);
    
    // The contact normal depends on whether we're extending or compressing
    if (currentLen > length) { // Extending
        contact->contactNormal = normal;
        contact->penetration = currentLen - length;
    }
    else { // Compressing
        contact->contactNormal = normal * glm::vec3(-1);
        contact->penetration = length - currentLen;
    }
    // Always use zero restitution for no bounciness
    contact->restitution = 0;
    return 1;
}