//
//  Particle.cpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/7/16.
//
//

#include "Particle.hpp"
#include <vector>
#include <memory>
#include <Eigen/Dense>

using namespace std;

Particle::Particle(Eigen::Vector3f position, Eigen::Vector3f velocity, Eigen::Vector3f color, float life, float size)
{
    this->position = position;
    this->velocity = velocity;
    this->color = color;
    this->life = life;
    this->size = size;
}

Particle::~Particle()
{
    
}

Eigen::Vector3f Particle::getColor()
{
    return this->color;
}

Eigen::Vector3f Particle::getPosition()
{
    return this->position;
}

Eigen::Vector3f Particle::getVelocity()
{
    return this->velocity;
}

float Particle::getSize()
{
    return this->size;
}

bool Particle::isDead()
{
    return this->life <= 0;
}

void Particle::setEmittedParticles(vector<shared_ptr<Particle>> emitted)
{
    this->emittedParticles = emitted;
}

std::vector<std::shared_ptr<Particle>> Particle::getEmittedParticles()
{
    std::vector<std::shared_ptr<Particle>> temp(this->emittedParticles);
    this->emittedParticles.clear();
    return temp;
}