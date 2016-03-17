//
//  SimpleParticle.hpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/7/16.
//
//

#ifndef SimpleParticle_hpp
#define SimpleParticle_hpp

#include <stdio.h>

#include "Particle.hpp"

class SimpleParticle : public Particle
{
public:
    SimpleParticle(Eigen::Vector3f position, Eigen::Vector3f velocity, Eigen::Vector3f color);
    ~SimpleParticle();
    void update(float time);
};

#endif /* SimpleParticle_hpp */
