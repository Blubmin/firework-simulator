//
//  TrailEmitterParticle.cpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/8/16.
//
//

#include <memory>
#include <cmath>

#include "TrailEmitterParticle.hpp"
#include "TrailParticle.hpp"
#include "Particle.hpp"

TrailEmitterParticle::TrailEmitterParticle(Eigen::Vector3f position, Eigen::Vector3f velocity, Eigen::Vector3f color, float life, float size)
 : Particle(position, velocity, color, life, size)
{
    stepRemaining = 0;
    stepSize = .01;
}

TrailEmitterParticle::~TrailEmitterParticle()
{
    
}

void TrailEmitterParticle::update(float time)
{
    stepRemaining += time;
    this->velocity += Eigen::Vector3f(0, -.2, 0) * time - .9 * this->velocity * time;
    while (stepRemaining - stepSize > 0)
    {
        this->position += this->velocity * stepSize;
        emittedParticles.push_back(std::dynamic_pointer_cast<Particle, TrailParticle>(std::make_shared<TrailParticle>(this->position, Eigen::Vector3f::Zero(), this->color, 2, this->size)));
        stepRemaining -= stepSize;
    }
    if (time > 0)
        this->position += this->velocity * stepRemaining;
    
    life -= time;
    // TODO use acutal min funciton
    this->size = this->size * (life < 3 ? life / 3 : 1.0);
    
    
}