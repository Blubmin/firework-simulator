#pragma once
#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <string>
#include <vector>
#include <memory>

class Program;
class Skeleton;

class Shape
{
public:
	Shape();
	virtual ~Shape();
	void loadMesh(const std::string &meshName);
	void init();
    void bind(std::shared_ptr<Program> prog, int frame);
    void unbind(std::shared_ptr<Program> prog);
	void draw(const std::shared_ptr<Program> prog, int frame, bool isCPU) const;
    void loadWeights(const std::string &fileName);
    void loadAnimation(const std::string &fileName);
	

    std::shared_ptr<Skeleton> skeleton;
private:
	std::vector<unsigned int> eleBuf;
	std::vector<float> posBuf;
    mutable std::vector<float> newPosBuf;
	std::vector<float> norBuf;
	std::vector<float> texBuf;
	unsigned eleBufID;
    unsigned posBufID;
	mutable unsigned newPosBufID;
	unsigned norBufID;
	unsigned texBufID;
};

#endif
