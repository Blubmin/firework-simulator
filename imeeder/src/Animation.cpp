//
//  Animation.cpp
//  Assignment2
//
//  Created by Ian Meeder on 2/9/16.
//
//

#include "Animation.hpp"
#include "Program.h"
#include <fstream>
#include <memory>
#include "GLSL.h"

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

Animation::Animation()
{
    
}

Animation::~Animation()
{
    
}

void Animation::loadAnimation(const std::string &fileName)
{
    std::ifstream file;
    file.open(fileName);
    
    std::string line;
    int nBones = 0;
    
    if (file.is_open())
    {
        while(std::getline(file, line))
        {
            char * temp = strdup(line.c_str());
            
            if (temp[0] == '#')
                continue;
            else if (!nBones)
            {
                char* tok = std::strtok(temp, " ");
                tok = std::strtok(NULL, " ");
                nBones = std::stoi(tok);
            }
            else
            {
                std::vector<Eigen::Matrix4f> matVec;
                char* tok = std::strtok(temp, " ");
                while (tok != NULL)
                {
                    float qx = std::stof(tok);
                    float qy = std::stof(std::strtok(NULL, " "));
                    float qz = std::stof(std::strtok(NULL, " "));
                    float qw = std::stof(std::strtok(NULL, " "));
                    float px = std::stof(std::strtok(NULL, " "));
                    float py = std::stof(std::strtok(NULL, " "));
                    float pz = std::stof(std::strtok(NULL, " "));
                    tok = std::strtok(NULL, " ");
                    
                    Eigen::Quaternionf quat;
                    quat.vec() << qx, qy, qz;
                    quat.w() = qw;
                    
                    Eigen::Vector3f point;
                    point << px, py, pz;
                    
                    Eigen::Matrix4f E = Eigen::Matrix4f::Identity();
                    E.block<3,3>(0,0) = quat.toRotationMatrix();
                    E.block<3,1>(0,3) = point;
                    
                    matVec.push_back(E);
                }
                
                if (bindPoses.size() == 0)
                {
                    bindPoses = matVec;
                    
                    for (int i = 0; i < matVec.size(); i++)
                    {
                        bindInverses.push_back(matVec[i].inverse());
                    }
                }
                else
                {
                    animationFrames.push_back(matVec);
                }
            }
        }
    }
    
    file.close();
}

Eigen::Matrix4f Animation::getBoneAnimation(int frame, int bone)
{
    return this->animationFrames[frame][bone];
}

void Animation::init()
{
    
}

void Animation::bind(std::shared_ptr<Program> prog, int frame)
{
    glUniformMatrix4fv(prog->getUniform("boneTransforms[0]"), 18, GL_FALSE, animationFrames[frame][0].data());
    glUniformMatrix4fv(prog->getUniform("bindPoses[0]"), 18, GL_FALSE, bindInverses[0].data());
    
    GLSL::checkError(GET_FILE_LINE);
}