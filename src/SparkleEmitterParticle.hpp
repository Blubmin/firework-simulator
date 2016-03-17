//
//  SparkleEmitterParticle.hpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/9/16.
//
//

#ifndef SparkleEmitterParticle_hpp
#define SparkleEmitterParticle_hpp

#include <stdio.h>

#include "Particle.hpp"

class SparkleEmitterParticle : public Particle
{
public:
    SparkleEmitterParticle(Eigen::Vector3f position, Eigen::Vector3f velocity, Eigen::Vector3f color, float life, float size);
    ~SparkleEmitterParticle();
    void update(float time);
    
private:
    float stepRemaining;
    float stepSize;
};

#endif /* SparkleEmitterParticle_hpp */
