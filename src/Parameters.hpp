//
//  Parameters.hpp
//  FireworksSimulator
//
//  Created by Ian Meeder on 3/14/16.
//
//

#ifndef Parameters_h
#define Parameters_h

#include <string>
#include <vector>

typedef enum { REGULAR, SPARKLE, SHAPE } FireworkType;
extern std::string shapeFile;
extern FireworkType type;
extern float particleColor[3];
extern float launchSpeed;
extern float explosionIntensity;
extern float explosionDuration;
extern float playbackSpeed;
extern std::vector<float> fileVertices;
extern float launchDirection[3];

#endif /* Parameters_h */
