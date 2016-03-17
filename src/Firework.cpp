//
//  Firework.cpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/7/16.
//
//

#include <algorithm>
#include <iostream>
#include <vector>

#include "Parameters.hpp"
#include "StandardExplosion.hpp"
#include "ShapeExplosion.hpp"
#include "SparkleExplosion.hpp"
#include "Firework.hpp"
#include "InitialParticle.hpp"
#include "Particle.hpp"

using namespace std;

Firework::Firework() : VAO(-1), posBuf(-1), colBuf(-1), eleBuf(-1)
{
    shared_ptr<InitialParticle> p = make_shared<InitialParticle>(Eigen::Vector3f(launchDirection) * launchSpeed / 10.f, explosionIntensity / 10.0f);
    switch (type) {
        case REGULAR:
            p->setExplosion(dynamic_pointer_cast<Explosion, StandardExplosion>(make_shared<StandardExplosion>(p)));
            break;
        case SHAPE:
            p->setExplosion(dynamic_pointer_cast<Explosion, ShapeExplosion>(make_shared<ShapeExplosion>(p)));
            break;
        case SPARKLE:
            p->setExplosion(dynamic_pointer_cast<Explosion, SparkleExplosion>(make_shared<SparkleExplosion>(p)));
        default:
            break;
    }
    this->particles.push_back(dynamic_pointer_cast<Particle, InitialParticle>(p));
    glInit();
}

Firework::~Firework()
{
    
}

void Firework::glInit()
{
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &this->posBuf);
    glBindBuffer(GL_ARRAY_BUFFER, this->posBuf);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glGenBuffers(1, &this->colBuf);
    glBindBuffer(GL_ARRAY_BUFFER, this->colBuf);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glGenBuffers(1, &this->sizeBuf);
    glBindBuffer(GL_ARRAY_BUFFER, this->sizeBuf);
    glBufferData(GL_ARRAY_BUFFER, sizes.size() * sizeof(float), &sizes[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
    
    glGenBuffers(1, &this->eleBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eleBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);
}


// Function to remove dead particles from the particle list
bool isDead(shared_ptr<Particle> p)
{
    return p->isDead();
}

void Firework::update(float time)
{
    for(int i = 0; i < particles.size(); i++)
    {
        particles[i]->update(time);
    }
    
    for (int i = 0; i < particles.size(); i++)
    {
        vector<shared_ptr<Particle>> temp(particles[i]->getEmittedParticles());
        particles.insert(particles.end(), temp.begin(), temp.end());
    }
    
    particles.erase(remove_if(particles.begin(), particles.end(), isDead), particles.end());
}

void Firework::draw(std::shared_ptr<Program> prog)
{
    positions.clear();
    colors.clear();
    indices.clear();
    sizes.clear();
    
    for (int i = 0; i < particles.size(); i++)
    {
        Eigen::Vector3f position = particles[i]->getPosition();
        Eigen::Vector3f color = particles[i]->getColor();
        positions.push_back(position(0));
        positions.push_back(position(1));
        positions.push_back(position(2));
        colors.push_back(color(0));
        colors.push_back(color(1));
        colors.push_back(color(2));
        sizes.push_back(particles[i]->getSize());
        indices.push_back(i);
    }
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->posBuf);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->colBuf);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->sizeBuf);
    glBufferData(GL_ARRAY_BUFFER, sizes.size() * sizeof(float), &sizes[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eleBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool Firework::isOver()
{
    return this->particles.size() == 0;
}