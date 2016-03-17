//
//  InitialParticle.cpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/7/16.
//
//

#include <Eigen/Dense>
#include <iostream>
#include "InitialParticle.hpp"
#include "Explosion.hpp"
#include "Particle.hpp"
#include "SimpleParticle.hpp"
#include "TrailEmitterParticle.hpp"
#include "SparkleEmitterParticle.hpp"

using namespace std;

InitialParticle::InitialParticle(Eigen::Vector3f velocity, float intensity)
: Particle(Eigen::Vector3f::Zero(), velocity, Eigen::Vector3f(1.0, .5, 1.0), 1, .05), intensity(intensity)
{

}

InitialParticle::~InitialParticle()
{
    
}

void InitialParticle::setExplosion(std::shared_ptr<Explosion> explosion)
{
    this->explosion = explosion;
}

float InitialParticle::getIntensity()
{
    return intensity;
}

void InitialParticle::update(float time)
{
    this->velocity += Eigen::Vector3f(0, -.98, 0) * time;
    this->position += this->velocity * time;
    
    if (velocity(1) < 0)
    {
        life = -1;
        this->explosion->explode();
    }
}