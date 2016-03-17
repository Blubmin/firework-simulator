//
//  SparkleParticle.hpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/9/16.
//
//

#ifndef SparkleParticle_hpp
#define SparkleParticle_hpp

#include <stdio.h>
#include <Eigen/Dense>
#include "Particle.hpp"

class SparkleParticle : public Particle
{
public:
    SparkleParticle(Eigen::Vector3f position, Eigen::Vector3f velocity, Eigen::Vector3f color, float life, float size);
    ~SparkleParticle();
    void update(float time);
    
private:
    float maxLife;
    float maxSize;
    float gravity;
};

#endif /* SparkleParticle_hpp */
