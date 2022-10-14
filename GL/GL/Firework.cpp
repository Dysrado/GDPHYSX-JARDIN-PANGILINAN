#include "Firework.h"

Firework::Firework(int level)
{
	fireworkLevel = level; // how many bursts
	isActive = true; // determines if the particle should be rendered
}

void Firework::initVariables(glm::vec3 pos, glm::vec3 size, glm::vec3 rot, int type)
{
	Particle::initVariables(pos, size, rot, type);

	age = rand() % 3; // chooses how long the lifespan
}


void Firework::update(float deltaTime, std::vector<Firework*>* list) // updates the firework list found in main
{
	if (age <= 0.f && isActive) { // age of the firework
		isActive = false;
		if (fireworkLevel > 0) {
			Firework* temp1 = new Firework(fireworkLevel - 1);
			temp1->initVariables(position, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 1);
			temp1->init();
			list->push_back(temp1);

			Firework* temp2 = new Firework(fireworkLevel - 1);
			temp2->initVariables(position, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 4);
			temp2->init();
			list->push_back(temp2);
		}
	}
	else { // decrement the fireworks age
		age -= deltaTime;
	}
}

void Firework::render(GLuint shaderProgram)
{
	if (isActive) {
		Particle::render(shaderProgram);
	}
}




