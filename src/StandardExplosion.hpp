//
//  StandardExplosion.hpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/13/16.
//
//

#ifndef StandardExplosion_hpp
#define StandardExplosion_hpp

#include <memory>
#include <stdio.h>
#include "Explosion.hpp"
#include "InitialParticle.hpp"

class StandardExplosion : public Explosion
{
public:
    StandardExplosion(std::shared_ptr<InitialParticle> initialParticle);
    ~StandardExplosion();
    void explode();
};

#endif /* StandardExplosion_hpp */
