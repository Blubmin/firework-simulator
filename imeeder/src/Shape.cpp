#include "Shape.h"
#include <iostream>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

#include "GLSL.h"
#include "Program.h"
#include "Skeleton.hpp"
#include "Animation.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;

Shape::Shape() :
	eleBufID(0),
    posBufID(0),
	newPosBufID(0),
	norBufID(0),
	texBufID(0)
{
    this->skeleton = std::make_shared<Skeleton>();
}

Shape::~Shape()
{
}

void Shape::loadMesh(const string &meshName)
{
	// Load geometry
	// Some obj files contain material information.
	// We'll ignore them for this assignment.
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> objMaterials;
	string errStr;
	bool rc = tinyobj::LoadObj(shapes, objMaterials, errStr, meshName.c_str());
	if(!rc) {
		cerr << errStr << endl;
	} else {
		posBuf = shapes[0].mesh.positions;
        newPosBuf = shapes[0].mesh.positions;
		norBuf = shapes[0].mesh.normals;
		texBuf = shapes[0].mesh.texcoords;
		eleBuf = shapes[0].mesh.indices;
	}
}

void Shape::init()
{
    // Send the position array to the GPU
    glGenBuffers(1, &posBufID);
    glBindBuffer(GL_ARRAY_BUFFER, posBufID);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
    
    // Send the position array to the GPU
	glGenBuffers(1, &newPosBufID);
	glBindBuffer(GL_ARRAY_BUFFER, newPosBufID);
	glBufferData(GL_ARRAY_BUFFER, newPosBuf.size()*sizeof(float), &newPosBuf[0], GL_STATIC_DRAW);
	
	// Send the normal array to the GPU
	if(!norBuf.empty()) {
		glGenBuffers(1, &norBufID);
		glBindBuffer(GL_ARRAY_BUFFER, norBufID);
		glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	}
	
	// Send the texture array to the GPU
	if(!texBuf.empty()) {
		glGenBuffers(1, &texBufID);
		glBindBuffer(GL_ARRAY_BUFFER, texBufID);
		glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW);
	}
	
	// Send the element array to the GPU
	glGenBuffers(1, &eleBufID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, eleBuf.size()*sizeof(unsigned int), &eleBuf[0], GL_STATIC_DRAW);
	
	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
    GLSL::checkError(GET_FILE_LINE);
    
    skeleton->init();
}

void Shape::bind(shared_ptr<Program> prog, int frame)
{
    skeleton->bind(prog, frame);
}

void Shape::unbind(std::shared_ptr<Program> prog)
{
    skeleton->unbind(prog);
}

void Shape::loadWeights(const std::string &fileName)
{
    this->skeleton->loadWeights(fileName);
}

void Shape::loadAnimation(const std::string &fileName)
{
    this->skeleton->loadAnimation(fileName);
}

void Shape::draw(const shared_ptr<Program> prog, int frame, bool isCPU) const
{
    for (unsigned i = 0; isCPU && i < posBuf.size(); i += 3)
    {
        Eigen::Vector4f point;
        point << 0, 0, 0, 0;
        for (int j = 0; j < skeleton->boneNum[int(i/3)]; j++)
        {
            float weight = this->skeleton->weights[int(i/3) * 16 + j];
            int boneIndex = this->skeleton->bonesIndices[int(i/3) * 16 + j];
            Eigen::Vector4f temp = this->skeleton->animation->bindPoses[boneIndex].inverse() * Eigen::Vector4f(posBuf[i], posBuf[i + 1], posBuf[i + 2], 1);
            temp = this->skeleton->animation->animationFrames[frame][boneIndex] * temp;
            temp = weight * temp;
            point += temp;
        }
        this->newPosBuf[i] = point(0);
        this->newPosBuf[i + 1] = point(1);
        this->newPosBuf[i + 2] = point(2);
    }
    
    // Send the position array to the GPU
    glGenBuffers(1, &newPosBufID);
    glBindBuffer(GL_ARRAY_BUFFER, newPosBufID);
    glBufferData(GL_ARRAY_BUFFER, newPosBuf.size()*sizeof(float), &newPosBuf[0], GL_STATIC_DRAW);
    
	// Bind position buffer
	int h_pos = prog->getAttribute("vertPos");
	GLSL::enableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, newPosBufID);
	glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
	
    // Bind position buffer
    int h_reg_pos = prog->getAttribute("vertRegPos");
    GLSL::enableVertexAttribArray(h_reg_pos);
    glBindBuffer(GL_ARRAY_BUFFER, posBufID);
    glVertexAttribPointer(h_reg_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
    
	// Bind normal buffer
	int h_nor = prog->getAttribute("vertNor");
	if(h_nor != -1 && norBufID != 0) {
		GLSL::enableVertexAttribArray(h_nor);
		glBindBuffer(GL_ARRAY_BUFFER, norBufID);
		glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
	}
	
	// Bind element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID);
	
	// Draw
	glDrawElements(GL_TRIANGLES, (int)eleBuf.size(), GL_UNSIGNED_INT, (const void *)0);
	
	if(h_nor != -1) {
		GLSL::disableVertexAttribArray(h_nor);
	}
	GLSL::disableVertexAttribArray(h_pos);
    GLSL::disableVertexAttribArray(h_reg_pos);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	GLSL::checkError(GET_FILE_LINE);
}
