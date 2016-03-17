//
//  Explosion.cpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/13/16.
//
//

#include "Explosion.hpp"
#include "InitialParticle.hpp"

using namespace std;

Explosion::Explosion(shared_ptr<InitialParticle> ip)
{
    this->initialParticle = ip;
}

Explosion::~Explosion()
{
    
}