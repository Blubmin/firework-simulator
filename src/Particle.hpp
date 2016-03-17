//
//  Particle.hpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/7/16.
//
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include <Eigen/Dense>
#include <vector>
#include <memory>

class Particle
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Particle(Eigen::Vector3f postiion, Eigen::Vector3f velocity, Eigen::Vector3f color, float life, float size);
    ~Particle();
    virtual void update(float time) = 0;
    Eigen::Vector3f getPosition();
    Eigen::Vector3f getVelocity();
    Eigen::Vector3f getColor();
    float getSize();
    void setEmittedParticles(std::vector<std::shared_ptr<Particle>> emitted);
    std::vector<std::shared_ptr<Particle>> getEmittedParticles();
    bool isDead();
protected:
    std::vector<std::shared_ptr<Particle>> emittedParticles;
    Eigen::Vector3f position;
    Eigen::Vector3f velocity;
    Eigen::Vector3f color;
    float size;
    float life;
};

#endif /* Particle_hpp */
