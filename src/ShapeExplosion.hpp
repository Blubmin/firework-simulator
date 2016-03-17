//
//  ShapeExplosion.hpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/14/16.
//
//

#ifndef ShapeExplosion_hpp
#define ShapeExplosion_hpp

#include <stdio.h>
#include <memory>
#include "Explosion.hpp"

class InitialParticle;

class ShapeExplosion : public Explosion
{
public:
    ShapeExplosion(std::shared_ptr<InitialParticle> particle);
    ~ShapeExplosion();
    void explode();
};

#endif /* ShapeExplosion_hpp */
