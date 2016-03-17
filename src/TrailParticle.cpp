//
//  TrailParticle.cpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/8/16.
//
//

#include "TrailParticle.hpp"
#include "Particle.hpp"


TrailParticle::TrailParticle(Eigen::Vector3f position, Eigen::Vector3f velocity, Eigen::Vector3f color, float life, float size)
    : Particle(position, velocity, color, life, size)
{
    this->maxLife = life;
    this->maxSize = size;
}
TrailParticle::~TrailParticle()
{
    
}

void TrailParticle::update(float time)
{
    life -= time * 5;
    size = maxSize * life / maxLife;
}
