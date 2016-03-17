//
//  Animation.hpp
//  Assignment2
//
//  Created by Ian Meeder on 2/9/16.
//
//

#ifndef Animation_hpp
#define Animation_hpp

#include <stdio.h>
#include <string>
#include <memory>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>
#include <vector>

class Program;

class Animation
{
public:
    std::vector<Eigen::Matrix4f> bindPoses;
    std::vector<Eigen::Matrix4f> bindInverses;
    std::vector<std::vector<Eigen::Matrix4f>> animationFrames;
    Animation();
    ~Animation();
    void loadAnimation(const std::string &fileName);
    Eigen::Matrix4f getBoneAnimation(int frame, int bone);
    void init();
    void bind(std::shared_ptr<Program> prog, int frame);
};

#endif /* Animation_hpp */
