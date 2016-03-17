//
//  Firework.hpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/7/16.
//
//

#ifndef Firework_hpp
#define Firework_hpp

#include <Eigen/Dense>
#include <memory>
#include <stdio.h>
#include <vector>
#include "Particle.hpp"
#include "Program.h"

class Firework
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Firework();
    ~Firework();
    void update(float time);
    void draw(std::shared_ptr<Program> prog);
    bool isOver();
private:
    std::vector<std::shared_ptr<Particle>> particles;
    
    GLuint VAO;
    GLuint posBuf;
    GLuint colBuf;
    GLuint sizeBuf;
    GLuint eleBuf;
    
    std::vector<float> positions;
    std::vector<float> colors;
    std::vector<float> sizes;
    std::vector<unsigned int> indices;
    
    void glInit();
};

#endif /* Firework_hpp */
