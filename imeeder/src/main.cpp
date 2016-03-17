#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "Texture.h"
#include "Cheb.hpp"
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;
using namespace Eigen;

bool keyToggles[256] = {false}; // only for English keyboards!

GLFWwindow *window; // Main application window
string RESOURCE_DIR = ""; // Where the resources are loaded from

shared_ptr<Program> progSimple;
shared_ptr<Program> progTex;
shared_ptr<Program> progCPU;
shared_ptr<Camera> camera;
shared_ptr<Shape> shape;
shared_ptr<Texture> texture;
shared_ptr<Cheb> cheb;

Vector2f mouse;

static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

static void char_callback(GLFWwindow *window, unsigned int key)
{
	keyToggles[key] = !keyToggles[key];
}

static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if(state == GLFW_PRESS) {
		bool altL = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS;
		bool altR = glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS;
		bool alt = altL || altR;
		if(alt) {
			// Move the links
			if(mouse(0) == 0.0f && mouse(1) == 0.0f) {
				// Initial call
				mouse << xmouse, ymouse;
			}
			float dx = xmouse - mouse(0);
			float dy = ymouse - mouse(1);
			float s = 1.0f;
			//
			// Use dx and dy to change the joint angles
			//
			cout << dx << " " << dy << endl;
			mouse << xmouse, ymouse;
		} else {
			camera->mouseMoved(xmouse, ymouse);
		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Get the current mouse position.
	double xmouse, ymouse;
	glfwGetCursorPos(window, &xmouse, &ymouse);
	// Get current window size.
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if(action == GLFW_PRESS) {
		bool shift = mods & GLFW_MOD_SHIFT;
		bool ctrl  = mods & GLFW_MOD_CONTROL;
		bool alt   = mods & GLFW_MOD_ALT;
		camera->mouseClicked(xmouse, ymouse, shift, ctrl, alt);
	}
}

static void init(string meshFile, string attachmentFile, string skeletonFile)
{
	GLSL::checkVersion();
	
	// Set background color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);
	
	keyToggles[(unsigned)'c'] = true;
	
    progCPU = make_shared<Program>();
    progCPU->setShaderNames(RESOURCE_DIR + "cpu_vert.glsl", RESOURCE_DIR + "cpu_frag.glsl");
    progCPU->setVerbose(true);
    progCPU->init();
	
	shape = make_shared<Shape>();
	shape->loadMesh(meshFile);
	
    cheb = make_shared<Cheb>();
    cheb->setShape(shape);
    cheb->loadWeights(attachmentFile);
    cheb->loadAnimation(skeletonFile);
    cheb->init();
    
	camera = make_shared<Camera>();
	
	// Initialize time.
	glfwSetTime(0.0);
    
    MatrixStack stack = MatrixStack();
    stack.pushMatrix();
	
	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	GLSL::checkError(GET_FILE_LINE);
}

void drawGrid(int rows, int cols, int delta)
{
    glColor3f(.6, .6, .6);
    glBegin(GL_LINES);
    
    for(int i = -rows / 2; i <= rows / 2; i += delta)
    {
        glVertex3i(-cols / 2, 0, i);
        glVertex3i(cols / 2, 0, i);
    }
    
    for(int j = -cols / 2; j <= cols / 2; j += delta)
    {
        glVertex3i(j, 0, -rows / 2);
        glVertex3i(j, 0, rows / 2);
    }
    glEnd();
}

void render()
{
    static time_point<system_clock> startTime = system_clock::now();
    duration<double> elapsed_seconds = system_clock::now() - startTime;
    float time = elapsed_seconds.count();
    
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	// Use the window size for camera.
	glfwGetWindowSize(window, &width, &height);
	camera->setAspect((float)width/(float)height);
	
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(keyToggles[(unsigned)'c']) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
	if(keyToggles[(unsigned)'l']) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	
	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	MV->pushMatrix();
	camera->applyViewMatrix(MV);
	
	// Draw grid
    // Setup the projection matrix
    GLSL::checkError(GET_FILE_LINE);
    glMatrixMode(GL_PROJECTION);
    GLSL::checkError(GET_FILE_LINE);
    glPushMatrix();
    glLoadMatrixf(P->topMatrix().data());
    
    // Setup the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(MV->topMatrix().data());
    drawGrid(6, 6, 1);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    bool isCpu = keyToggles['g'] ? 0 : 1;
    
	// Draw shape
	progCPU->bind();
	glUniformMatrix4fv(progCPU->getUniform("P"), 1, GL_FALSE, P->topMatrix().data());
    glUniformMatrix4fv(progCPU->getUniform("MV"), 1, GL_FALSE, MV->topMatrix().data());
    
    glUniform1i(progCPU->getUniform("isCPU"), isCpu);
    
    cheb->bind(progCPU, time);
    cheb->draw(progCPU, time, isCpu);
    cheb->unbind(progCPU);
    
	progCPU->unbind();
	
	//////////////////////////////////////////////////////
	// Cleanup
	//////////////////////////////////////////////////////
	
	// Pop stacks
	MV->popMatrix();
	P->popMatrix();
	
	GLSL::checkError(GET_FILE_LINE);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Please specify the resource directory." << endl;
		return 0;
	}
	RESOURCE_DIR = argv[1] + string("/");
	
	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(640, 480, "Assignment 2 - Ian Meeder", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}
	// Make the window's context current.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	glGetError(); // A bug in glewInit() causes an error that we can safely ignore.
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	// Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
	// Set char callback.
	glfwSetCharCallback(window, char_callback);
	// Set cursor position callback.
	glfwSetCursorPosCallback(window, cursor_position_callback);
	// Set mouse button callback.
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// Initialize scene.
	init(argv[2], argv[3], argv[4]);
	// Loop until the user closes the window.
	while(!glfwWindowShouldClose(window)) {
		// Render scene.
		render();
		// Swap front and back buffers.
		glfwSwapBuffers(window);
		// Poll for and process events.
		glfwPollEvents();
	}
	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
