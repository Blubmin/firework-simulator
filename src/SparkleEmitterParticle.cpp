//
//  SparkleEmitterParticle.cpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/9/16.
//
//

#include "Parameters.hpp"
#include "SparkleEmitterParticle.hpp"
#include "SparkleParticle.hpp"
#include "Particle.hpp"

SparkleEmitterParticle::SparkleEmitterParticle(Eigen::Vector3f position, Eigen::Vector3f velocity, Eigen::Vector3f color, float life, float size)
: Particle(position, velocity, color, life, size)
{
    stepRemaining = 0;
    stepSize = .05;
}

SparkleEmitterParticle::~SparkleEmitterParticle()
{
    
}

void SparkleEmitterParticle::update(float time)
{
    stepRemaining += time;
    this->velocity += Eigen::Vector3f(0, -.2, 0) * time - .9 * this->velocity * time;
    while (stepRemaining - stepSize > 0)
    {
        this->position += this->velocity * stepSize;
        emittedParticles.push_back(std::dynamic_pointer_cast<Particle, SparkleParticle>(std::make_shared<SparkleParticle>(this->position, Eigen::Vector3f::Zero(), Eigen::Vector3f(particleColor), 2, this->size * .5)));
        stepRemaining -= stepSize;
    }
    if (time > 0)
        this->position += this->velocity * stepRemaining;
    
    life -= time;
    // TODO use acutal min funciton
    this->size = this->size * (life < 3 ? life / 3 : 1.0);
}