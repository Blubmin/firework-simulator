//
//  Skeleton.hpp
//  Assignment2
//
//  Created by Ian Meeder on 2/9/16.
//
//

#ifndef Skeleton_hpp
#define Skeleton_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
#include <Eigen/Dense>

class Animation;
class Program;

class Skeleton
{
    unsigned weightBufID;
    unsigned boneIdxID;
    unsigned boneNumID;
public:
    std::vector<float> weights;
    std::vector<float> bonesIndices;
    std::vector<float> boneNum;
    std::shared_ptr<Animation> animation;
    Skeleton();
    ~Skeleton();
    void loadWeights(const std::string &fileName);
    void loadAnimation(const std::string &fileName);
    Eigen::Matrix4f getAnimationTransform(int frame, int vertex);
    void init();
    void bind(std::shared_ptr<Program> prog, int frame);
    void unbind(std::shared_ptr<Program> prog);
};

#endif /* Skeleton_hpp */
