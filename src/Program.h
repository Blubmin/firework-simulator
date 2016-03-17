#pragma  once
#ifndef __Program__
#define __Program__

#include <map>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

class Program
{
public:
	Program();
	virtual ~Program();
	
	void setVerbose(bool v) { verbose = v; }
	bool isVerbose() const { return verbose; }
	
    void setShaderNames(const std::string &v, const std::string &f, const std::string &g = "");
	virtual bool init();
	virtual void bind();
	virtual void unbind();

	GLint getAttribute(const std::string &name) const;
	GLint getUniform(const std::string &name) const;
	
protected:
	std::string vShaderName;
	std::string fShaderName;
    std::string gShaderName;
	
private:
	GLuint pid;
	std::map<std::string,GLint> attributes;
	std::map<std::string,GLint> uniforms;
    std::map<std::string,GLint> uniformBlocks;
	bool verbose;
    void setupHandles();
};

#endif
