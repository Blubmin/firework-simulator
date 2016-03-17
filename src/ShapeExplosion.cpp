//
//  ShapeExplosion.cpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/14/16.
//
//

#include "ShapeExplosion.hpp"
#include "SimpleParticle.hpp"
#include "Parameters.hpp"
#include "InitialParticle.hpp"
#include <Eigen/Dense>

using namespace std;

ShapeExplosion::ShapeExplosion(shared_ptr<InitialParticle> ip)
: Explosion(ip)
{
    
}

ShapeExplosion::~ShapeExplosion()
{
    
}

void ShapeExplosion::explode()
{
    vector<shared_ptr<Particle>> emittedParticles;
    
    for (int i = 0; i < fileVertices.size(); i += 3)
    {
        emittedParticles.push_back(dynamic_pointer_cast<Particle, SimpleParticle>(make_shared<SimpleParticle>(this->initialParticle->getPosition(), Eigen::Vector3f(fileVertices[i], fileVertices[i + 1], fileVertices[i + 2]) * this->initialParticle->getIntensity() + this->initialParticle->getVelocity(), Eigen::Vector3f(particleColor))));
    }
    
    this->initialParticle->setEmittedParticles(emittedParticles);
}