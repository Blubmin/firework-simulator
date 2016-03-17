//
//  Cheb.cpp
//  Assignment2
//
//  Created by Ian Meeder on 2/9/16.
//
//

#include "Cheb.hpp"
#include "Shape.h"
#include "Skeleton.hpp"
#include "Animation.hpp"
#include <Eigen/Dense>
#include "GLSL.h"
#include "Program.h"

Cheb::Cheb()
{

}

Cheb::~Cheb()
{
    
}

void Cheb::setShape(std::shared_ptr<Shape> shape)
{
    this->shape = shape;
}

void Cheb::loadWeights(const std::string &fileName)
{
    this->shape->loadWeights(fileName);
}

void Cheb::loadAnimation(const std::string &fileName)
{
    this->shape->loadAnimation(fileName);
}

void Cheb::init()
{
    shape->init();
}

void Cheb::bind(std::shared_ptr<Program> prog, float time)
{
    int frame = int(time * 30) % shape->skeleton->animation->animationFrames.size();
    
    shape->bind(prog, frame);
}

void Cheb::unbind(std::shared_ptr<Program> prog)
{
    shape->unbind(prog);
}

// Draws the cheb. Little cheb. Mr chebbers.
void Cheb::draw(std::shared_ptr<Program> prog, float time, bool isCPU)
{
    glUniform3fv(prog->getUniform("diffuse"), 1, Eigen::Vector3f(.7, .2, 1).data());
    glUniform3fv(prog->getUniform("specular"), 1, Eigen::Vector3f(1, 1, 1).data());
    glUniform3fv(prog->getUniform("ambient"), 1, Eigen::Vector3f(.28, .06, .4).data());
    
    int frame = int(time * 30) % shape->skeleton->animation->animationFrames.size();
    
    shape->draw(prog, frame, isCPU);
}