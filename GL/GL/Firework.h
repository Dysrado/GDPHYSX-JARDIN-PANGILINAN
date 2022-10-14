#pragma once
#include "Particle.h"
class Firework :
    public Particle
{
public:
    Firework(int level);
    void initVariables(glm::vec3 pos, glm::vec3 size, glm::vec3 rot, int type);
    void update(float deltaTime, std::vector<Firework*>* list);
    void render(GLuint shaderProgram);

private:
    float age;
    bool isActive;
    int fireworkLevel;
};

