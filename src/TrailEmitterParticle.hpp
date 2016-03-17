//
//  TrailEmitterParticle.hpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/8/16.
//
//

#ifndef TrailEmitterParticle_hpp
#define TrailEmitterParticle_hpp

#include <stdio.h>
#include <Eigen/Dense>

#include "Particle.hpp"

class TrailEmitterParticle : public Particle
{
public:
    TrailEmitterParticle(Eigen::Vector3f position, Eigen::Vector3f velocity, Eigen::Vector3f color, float life, float size);
    ~TrailEmitterParticle();
    void update(float time);
    
private:
    float stepRemaining;
    float stepSize;
};

#endif /* TrailEmitterParticle_hpp */
