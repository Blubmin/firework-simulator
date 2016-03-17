//
//  Cheb.hpp
//  Assignment2
//
//  Created by Ian Meeder on 2/9/16.
//
//

#ifndef Cheb_hpp
#define Cheb_hpp

#include <stdio.h>
#include <memory>
#include <Eigen/Dense>

// Define classes without including the headers
class Shape;
class Program;
class Skeleton;

class Cheb
{
private:
    std::shared_ptr<Shape> shape;
    
public:
    Cheb();
    ~Cheb();
    void setShape(std::shared_ptr<Shape> shape);
    void loadWeights(const std::string &fileName);
    void loadAnimation(const std::string &fileName);
    void init();
    void bind(std::shared_ptr<Program> prog, float time);
    void unbind(std::shared_ptr<Program> prog);
    void draw(std::shared_ptr<Program> prog, float time, bool isCPU);
};

#endif /* Cheb_hpp */
