#include "ParticleLink.h"

float ParticleLink::currentLength() const {
	glm::vec3 relativePos = particle[0]->getPosition() - particle[1]->getPosition();
	return sqrt((relativePos.x * relativePos.x) + (relativePos.y * relativePos.y) + (relativePos.z * relativePos.z));
	
}
unsigned ParticleRod::addContact(ParticleContact* contact, unsigned limit) const
{
    float currentLen = currentLength();
    if (currentLen == length) {
        //std::cout << "Same Length\n";
        return 0;
    }

    //std::cout << "Current Len: " << currentLen << std::endl;
    //std::cout << "Given Length " << length << std::endl << std::endl;
    
    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    glm::vec3 normal = particle[1]->getPosition() - particle[0]->getPosition();
    normal = glm::normalize(normal);
   // std::cout << "Normal " << normal.y << std::endl;
    
    if (currentLen > length) {
        contact->contactNormal = normal;
        contact->penetration = currentLen - length;
        std::cout << "Extending...\n";
        std::cout << "currentLength: " << currentLen << " vs length: " << length << "\n\n";
    }
    else {
        contact->contactNormal = normal * glm::vec3(-1);
        contact->penetration = length - currentLen;

       // std::cout << "Shrinking...\n";
        //std::cout << "currentLength: " << currentLen << " vs length: " << length << "\n\n";
    }
    //std::cout << "Contact Normal = " << contact->contactNormal.y << std::endl;
    contact->restitution = 0;
   // std::cout << "Returning 1\n";
    return 1;
}