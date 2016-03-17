//
//  Explosion.hpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/13/16.
//
//

#ifndef Explosion_hpp
#define Explosion_hpp

#include <memory>
#include <stdio.h>

class InitialParticle;

class Explosion
{
public:
    Explosion(std::shared_ptr<InitialParticle> initalParticle);
    ~Explosion();
    virtual void explode() = 0;
    
protected:
    std::shared_ptr<InitialParticle> initialParticle;
};

#endif /* Explosion_hpp */
