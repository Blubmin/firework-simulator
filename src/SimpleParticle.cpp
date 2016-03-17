//
//  SimpleParticle.cpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/7/16.
//
//

#include "SimpleParticle.hpp"

SimpleParticle::SimpleParticle(Eigen::Vector3f position, Eigen::Vector3f velocity, Eigen::Vector3f color)
: Particle(position, velocity, color, 2, 0.05)
{

}

SimpleParticle::~SimpleParticle()
{
    
}

void SimpleParticle::update(float time)
{
    static float initialLife = this->life;
    static float initialSize = this->size;
    this->life -= time;
    this->size = initialSize * this->life / initialLife;
    this->velocity += Eigen::Vector3f(0, -.3, 0) * time - .9 * this->velocity * time;
    this->position += this->velocity * time;
}