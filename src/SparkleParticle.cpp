//
//  SparkleParticle.cpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/9/16.
//
//

#include "SparkleParticle.hpp"

SparkleParticle::SparkleParticle(Eigen::Vector3f position, Eigen::Vector3f velocity, Eigen::Vector3f color, float life, float size)
: Particle(position, velocity, color, life, size)
{
    this->maxLife = life + fmod(rand(), .01) * 50;
    this->maxSize = size;
    this->gravity = fmod(rand(), .01) * 30 - .3;
}

SparkleParticle::~SparkleParticle()
{
    
}

void SparkleParticle::update(float time)
{
    this->velocity += Eigen::Vector3f(0, this->gravity, 0) * time - .9 * this->velocity * time;
    this->position += this->velocity * time;
    life -= time;
    size = maxSize * cos(life * 10);
}