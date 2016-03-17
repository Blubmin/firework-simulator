//
//  SparkleExplosion.cpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/14/16.
//
//

#include "SparkleExplosion.hpp"
#include "Parameters.hpp"
#include "SparkleEmitterParticle.hpp"

using namespace std;

SparkleExplosion::SparkleExplosion(shared_ptr<InitialParticle> ip)
: Explosion(ip)
{
    
}

SparkleExplosion::~SparkleExplosion()
{
    
}

void SparkleExplosion::explode()
{
    vector<shared_ptr<Particle>> emittedParticles;
    
    for (int j = 0; j < 200; j++)
    {
        int x = rand() % 2000 - 1000;
        int y = rand() % 2000 - 1000;
        int z = rand() % 2000 - 1000;
        float speed = fmod(float(rand()), 0.01f) * 50 + this->initialParticle->getIntensity();
        
        emittedParticles.push_back(std::dynamic_pointer_cast<Particle, SparkleEmitterParticle>(std::make_shared<SparkleEmitterParticle>(this->initialParticle->getPosition(), Eigen::Vector3f(x, y, z).normalized() * speed, Eigen::Vector3f(particleColor), explosionDuration, this->initialParticle->getSize())));
    }
    
    this->initialParticle->setEmittedParticles(emittedParticles);
}