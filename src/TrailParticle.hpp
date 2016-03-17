//
//  TrailParticle.hpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/8/16.
//
//

#ifndef TrailParticle_hpp
#define TrailParticle_hpp

#include <stdio.h>
#include <Eigen/Dense>
#include "Particle.hpp"

class TrailParticle : public Particle
{
public:
    TrailParticle(Eigen::Vector3f position, Eigen::Vector3f velocity, Eigen::Vector3f color, float life, float size);
    ~TrailParticle();
    void update(float time);
    
private:
    float maxLife;
    float maxSize;
};

#endif /* TrailParticle_hpp */
