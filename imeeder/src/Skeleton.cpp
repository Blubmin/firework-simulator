//
//  Skeleton.cpp
//  Assignment2
//
//  Created by Ian Meeder on 2/9/16.
//
//

#include <iostream>
#include "Skeleton.hpp"
#include <cstring>
#include <fstream>
#include <cfloat>
#include "Animation.hpp"
#include "GLSL.h"
#include "Program.h"
#include <Eigen/Dense>

Skeleton::Skeleton() :
    weightBufID(0),
    boneIdxID(0)
{
    this->animation = std::make_shared<Animation>();
}

Skeleton::~Skeleton()
{
    
}

void Skeleton::loadWeights(const std::string &fileName)
{
    std::ifstream file;
    file.open(fileName);
    
    std::string line;
    int nBones = 0;
    
    if (file.is_open())
    {   
        while (std::getline(file, line))
        {
            char * temp = strdup(line.c_str());
            
            if (temp[0] == '#')
                continue;
            else if (!nBones)
            {
                std::string tok = std::strtok(temp, " ");
                tok = std::strtok(NULL, " ");
                nBones = std::stoi(tok);
            }
            else
            {
                std::vector<float> vertWeights;
                std::vector<float> boneIdx;
                char * tok = std::strtok(temp, " ");
                
                int idx = 0;
                
                while (tok != NULL)
                {
                    float weight = std::stof(std::string(tok));
                    if(weight > FLT_EPSILON)
                    {
                        vertWeights.push_back(weight);
                        boneIdx.push_back(idx);
                    }
                    
                    idx++;
                    tok = std::strtok(NULL, " ");
                }
                boneNum.push_back(vertWeights.size());
                vertWeights.resize(16, 0);
                weights.insert(weights.end(), vertWeights.begin(), vertWeights.end());
                boneIdx.resize(16, 0);
                bonesIndices.insert(bonesIndices.end(), boneIdx.begin(), boneIdx.end());
            }
        }
    }
    file.close();
}

void Skeleton::init()
{
    // Send the position array to the GPU
    glGenBuffers(1, &weightBufID);
    glBindBuffer(GL_ARRAY_BUFFER, weightBufID);
    glBufferData(GL_ARRAY_BUFFER, weights.size()*sizeof(float), &weights[0], GL_STATIC_DRAW);
    
    // Send the position array to the GPU
    glGenBuffers(1, &boneIdxID);
    glBindBuffer(GL_ARRAY_BUFFER, boneIdxID);
    glBufferData(GL_ARRAY_BUFFER, bonesIndices.size()*sizeof(float), &bonesIndices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &boneNumID);
    glBindBuffer(GL_ARRAY_BUFFER, boneNumID);
    glBufferData(GL_ARRAY_BUFFER, boneNum.size()*sizeof(float), &boneNum[0], GL_STATIC_DRAW);
    
    animation->init();
    
    // Unbind the arrays
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    GLSL::checkError(GET_FILE_LINE);

}

void Skeleton::bind(std::shared_ptr<Program> prog, int frame)
{
    GLSL::enableVertexAttribArray(prog->getAttribute("boneNum"));
    glBindBuffer(GL_ARRAY_BUFFER, boneNumID);
    glVertexAttribPointer(prog->getAttribute("boneNum"), 1, GL_FLOAT, GL_FALSE, 0, (const void *)0);
    
    GLSL::enableVertexAttribArray(prog->getAttribute("weights0"));
    GLSL::enableVertexAttribArray(prog->getAttribute("weights1"));
    GLSL::enableVertexAttribArray(prog->getAttribute("weights2"));
    GLSL::enableVertexAttribArray(prog->getAttribute("weights3"));
    glBindBuffer(GL_ARRAY_BUFFER, weightBufID);
    unsigned stride = 16*sizeof(float);
    glVertexAttribPointer(prog->getAttribute("weights0"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)( 0*sizeof(float)));
    glVertexAttribPointer(prog->getAttribute("weights1"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)( 4*sizeof(float)));
    glVertexAttribPointer(prog->getAttribute("weights2"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)( 8*sizeof(float)));
    glVertexAttribPointer(prog->getAttribute("weights3"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)(12*sizeof(float)));
    GLSL::checkError(GET_FILE_LINE);
    
    GLSL::enableVertexAttribArray(prog->getAttribute("bones0"));
    GLSL::enableVertexAttribArray(prog->getAttribute("bones1"));
    GLSL::enableVertexAttribArray(prog->getAttribute("bones2"));
    GLSL::enableVertexAttribArray(prog->getAttribute("bones3"));
    glBindBuffer(GL_ARRAY_BUFFER, boneIdxID);
    glVertexAttribPointer(prog->getAttribute("bones0"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)( 0*sizeof(float)));
    glVertexAttribPointer(prog->getAttribute("bones1"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)( 4*sizeof(float)));
    glVertexAttribPointer(prog->getAttribute("bones2"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)( 8*sizeof(float)));
    glVertexAttribPointer(prog->getAttribute("bones3"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)(12*sizeof(float)));
    
    GLSL::checkError(GET_FILE_LINE);
    
    animation->bind(prog, frame);
}

void Skeleton::unbind(std::shared_ptr<Program> prog)
{
    GLSL::disableVertexAttribArray(prog->getAttribute("weights0"));
    GLSL::disableVertexAttribArray(prog->getAttribute("weights1"));
    GLSL::disableVertexAttribArray(prog->getAttribute("weights2"));
    GLSL::disableVertexAttribArray(prog->getAttribute("weights3"));

    GLSL::disableVertexAttribArray(prog->getAttribute("bones0"));
    GLSL::disableVertexAttribArray(prog->getAttribute("bones1"));
    GLSL::disableVertexAttribArray(prog->getAttribute("bones2"));
    GLSL::disableVertexAttribArray(prog->getAttribute("bones3"));
}

void Skeleton::loadAnimation(const std::string &fileName)
{
    this->animation->loadAnimation(fileName);
}