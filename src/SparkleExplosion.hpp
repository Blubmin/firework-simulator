//
//  SparkleExplosion.hpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/14/16.
//
//

#ifndef SparkleExplosion_hpp
#define SparkleExplosion_hpp

#include <stdio.h>
#include <memory>
#include "Explosion.hpp"
#include "InitialParticle.hpp"

class SparkleExplosion : public Explosion
{
public:
    SparkleExplosion(std::shared_ptr<InitialParticle> initialParticle);
    ~SparkleExplosion();
    void explode();
};

#endif /* SparkleExplosion_hpp */
