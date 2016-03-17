//
//  InitialParticle.hpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/7/16.
//
//

#ifndef InitialParticle_hpp
#define InitialParticle_hpp

#include <Eigen/Dense>
#include <stdio.h>
#include <memory>
#include "Particle.hpp"

class Explosion;

class InitialParticle : public Particle
{
public:
    InitialParticle(Eigen::Vector3f velocity, float intensity);
    ~InitialParticle();
    void setExplosion(std::shared_ptr<Explosion> explosion);
    void update(float time);
    float getIntensity();
private:
    float intensity;
    std::shared_ptr<Explosion> explosion;
};

#endif /* InitialParticle_hpp */
