#include "ParticleContactResolver.h"

ParticleContactResolver::ParticleContactResolver(unsigned iterations)
{
	setIterations(iterations);
}

void ParticleContactResolver::setIterations(unsigned iterations)
{
	this->iterations = iterations;
}

void ParticleContactResolver::resolveContacts(ParticleContact* contactArray, unsigned numContacts, float duration)
{
	iterationsUsed = 0;
	while (iterationsUsed < iterations) {
		glm::vec3 max(0);
		unsigned maxIndex = numContacts;
		for (unsigned i = 0; i < numContacts; i++) {
			glm::vec3 sepVel = contactArray[i].calculateSeparatingVelocity();

			if (sepVel.x < max.x && sepVel.y < max.y && sepVel.z < max.z) {
				max = sepVel;
				maxIndex = i;
			}
		}

		contactArray[maxIndex].resolve(duration);
		iterationsUsed++;
	}
}
