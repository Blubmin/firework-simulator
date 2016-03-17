#include <iostream>
#include <vector>
#include <fstream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Firework.hpp"
#include "Parameters.hpp"
#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include <ctime>
#include <chrono>
#include <AntTweakBar.h>
#include "tinyfiledialogs.h"
#include <string>

using namespace std;
using namespace chrono;
using namespace Eigen;

bool keyToggles[256] = {false}; // only for English keyboards!

GLFWwindow *window; // Main application window
string RESOURCE_DIR = ""; // Where the resources are loaded from

shared_ptr<Program> progSimple;
shared_ptr<Program> progTex;
shared_ptr<Program> progCPU;
shared_ptr<Program> fwProg;
shared_ptr<Program> gridProg;
shared_ptr<Camera> camera;

TwBar* myBar;

Vector2f mouse;

shared_ptr<Firework> firework;

GLuint VAO;
GLuint elementBuf;
vector<unsigned int> indices;
GLuint pointBuf;
vector<GLfloat> points;

GLuint VAO2;
GLuint gridBuf;
vector<GLfloat> gridPoints;
GLuint gridElementBuf;
vector<unsigned int> gridIndices;

float particleColor[3] = {1.0f, 1.0f, 1.0f};
float launchSpeed = 30;
float launchDirection[3] = {0, 1.0f, 0};
float explosionIntensity = 30;
float explosionDuration = 5;
float playbackSpeed = 1;
vector<float> fileVertices;

char PLAY[] = "Play";
char PAUSE[] = "Pause";

FireworkType type = REGULAR;
TwEnumVal fireworkEV[] = { {REGULAR, "Normal"}, {SPARKLE, "Sparkle" }, {SHAPE, "Shape"} };
TwType fireworkType;

string shapeFile = "";
char * fileType = "*.obj";
char * const * const fileTypes = {&fileType};


static void pause_simulation(void *object)
{
    char* label = keyToggles[' '] ? PAUSE : PLAY;
    TwSetParam(myBar, "Play", "Label", TW_PARAM_CSTRING, 1, label);
    keyToggles[' '] = !keyToggles[' '];
}

static void loadFile(string filename)
{
    fileVertices.clear();
    ifstream myfile;
    myfile.open (filename);
    string line = "";
    if (myfile.is_open())
    {
        while(getline(myfile, line))
        {
            char * temp = strdup(line.c_str());
            char * pch;
            pch = strtok(temp, " ");
            if (pch != nullptr && strcmp(pch, "v") == 0)
            {
                fileVertices.push_back(stof(strtok(NULL, " ")));
                fileVertices.push_back(stof(strtok(NULL, " ")));
                fileVertices.push_back(stof(strtok(NULL, " ")));
            }
        }
    }
    myfile.close();
    
    
    float minx = 10;
    float miny = 10;
    float minz = 10;
    float maxz = -10;
    float maxx = -10;
    float maxy = -10;
    
    for (int i = 0 ; i < fileVertices.size(); i += 3)
    {
        minx = min(fileVertices[i], minx);
        miny = min(fileVertices[i + 1], miny);
        minz = min(fileVertices[i + 2], minz);
        maxx = max(fileVertices[i], maxx);
        maxy = max(fileVertices[i + 1], maxy);
        maxz = max(fileVertices[i + 2], maxz);
    }
    
    float bigSpread = 0;
    bigSpread = max(maxx - minx, bigSpread);
    bigSpread = max(maxy - miny, bigSpread);
    bigSpread = max(maxz - minz, bigSpread);
    
    for (int i = 0 ; i < fileVertices.size(); i += 3)
    {
        fileVertices[i] = ((fileVertices[i] - minx) / (maxx - minx) * 2 - 1) * ((maxx - minx) / bigSpread);
        fileVertices[i + 1] = ((fileVertices[i + 1] - miny) / (maxy - miny) * 2 - 1) * ((maxy - miny) / bigSpread);
        fileVertices[i + 2] = ((fileVertices[i + 2] - minz) / (maxz - minz) * 2 - 1) * ((maxz - minz) / bigSpread);
    }
}

static void browse(void *object)
{
    pause_simulation(nullptr);
    shapeFile = tinyfd_openFileDialog("Choose a Shape File", RESOURCE_DIR.c_str(), 1, fileTypes, "mesh file", 0);
    loadFile(shapeFile);
    std::size_t found = shapeFile.find_last_of("/\\");
    string temp = shapeFile.substr(found+1);
    shapeFile = temp;
    pause_simulation(nullptr);
}

static void initAntTweakBar()
{
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(1920, 685);
    myBar = TwNewBar("Firework Details");
    // Defining season enum type
    fireworkType = TwDefineEnum("FireworkType", fireworkEV, 3);
    // Adding season to bar
    TwAddVarRW(myBar, "Firework Type", fireworkType, &type, "group='Firework'");
    TwAddButton(myBar, "Browse...", browse, NULL, "group='Firework' visible='false'");
    TwAddVarRO(myBar, "Shape File", TW_TYPE_STDSTRING, &shapeFile, "group='Firework' visible='false'");
    TwAddVarRW(myBar, "Launch Speed", TW_TYPE_FLOAT, &launchSpeed, "group='Firework' min=0 max=100");
    TwAddVarRW(myBar, "Launch Direction", TW_TYPE_DIR3F, &launchDirection, "group='Firework'");
    TwAddVarRW(myBar, "Explosion Intensity", TW_TYPE_FLOAT, &explosionIntensity, "group='Firework' min=0 max=100");
    TwAddVarRW(myBar, "Explosion Duration", TW_TYPE_FLOAT, &explosionDuration, "group='Firework'");
    TwAddVarRW(myBar, "Color", TW_TYPE_COLOR3F, &particleColor, "group='Firework'");
    TwAddSeparator(myBar, "", "");
    TwAddButton(myBar, "Play", pause_simulation, NULL, "label='Pause' group='Playback' key=SPACE");
    TwAddVarRW(myBar, "Playback Speed", TW_TYPE_FLOAT, &playbackSpeed, "min=.5 max=5 step=.1");
}

static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (!TwEventKeyGLFW(key, action))
    {
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
}

static void char_callback(GLFWwindow *window, unsigned int key)
{
    if (!TwEventCharGLFW(key, GLFW_PRESS))
    {
        keyToggles[key] = !keyToggles[key];
    }
}

static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse)
{
    if(!TwEventMousePosGLFW(xmouse, ymouse))
    {
        int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if(state == GLFW_PRESS) {
            camera->mouseMoved(xmouse, ymouse);
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (!TwEventMouseButtonGLFW(button, action))
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
}

static void init()
{
	GLSL::checkVersion();
	
	// Set background color
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);
	
	keyToggles[(unsigned)'c'] = true;
    
    fwProg = make_shared<Program>();
    fwProg->setShaderNames(RESOURCE_DIR + "firework_vert.glsl", RESOURCE_DIR + "firework_frag.glsl", RESOURCE_DIR + "firework_geom.glsl");
    fwProg->setVerbose(true);
    fwProg->init();
    
    gridProg = make_shared<Program>();
    gridProg->setShaderNames(RESOURCE_DIR + "grid_vert.glsl", RESOURCE_DIR + "grid_frag.glsl");
    gridProg->setVerbose(true);
    gridProg->init();
    
	camera = make_shared<Camera>();
    
    firework = make_shared<Firework>();
    
    // Grid Binding
    gridPoints.push_back(-1);
    gridPoints.push_back(0);
    gridPoints.push_back(-1);
    gridPoints.push_back(1);
    gridPoints.push_back(0);
    gridPoints.push_back(-1);
    gridPoints.push_back(1);
    gridPoints.push_back(0);
    gridPoints.push_back(1);
    gridPoints.push_back(-1);
    gridPoints.push_back(0);
    gridPoints.push_back(1);
    
    gridIndices.push_back(0);
    gridIndices.push_back(1);
    gridIndices.push_back(2);
    gridIndices.push_back(3);
    
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);
    
    glGenBuffers(1, &gridBuf);
    glBindBuffer(GL_ARRAY_BUFFER, gridBuf);
    glBufferData(GL_ARRAY_BUFFER, gridPoints.size()*sizeof(float), &gridPoints[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glGenBuffers(1, &gridElementBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridElementBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gridIndices.size() * sizeof(float), &gridIndices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);
    
	// Initialize time.
	glfwSetTime(0.0);
    
    MatrixStack stack = MatrixStack();
    stack.pushMatrix();
	
	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	GLSL::checkError(GET_FILE_LINE);
}

void render()
{
    static time_point<system_clock> startTime = system_clock::now();
    duration<double> elapsed_seconds = system_clock::now() - startTime;
    static float prevTime = elapsed_seconds.count();
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
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	
	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	MV->pushMatrix();
	camera->applyViewMatrix(MV);
    
    // Draw Grid
    
    gridProg->bind();
    
    GLSL::checkError(GET_FILE_LINE);
    
    glUniformMatrix4fv(gridProg->getUniform("MV"), 1, GL_FALSE, MV->topMatrix().data());
    glUniformMatrix4fv(gridProg->getUniform("P"), 1, GL_FALSE, P->topMatrix().data());
    
    GLSL::checkError(GET_FILE_LINE);
    
    glBindVertexArray(VAO2);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridElementBuf);
    glDrawElements(GL_LINE_LOOP, gridIndices.size(), GL_UNSIGNED_INT, 0);
    
    GLSL::checkError(GET_FILE_LINE);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    gridProg->unbind();
    
    float timeDiff;
    if (keyToggles[' '])
        timeDiff = 0;
    else
        timeDiff = time - prevTime;
    firework->update(timeDiff * playbackSpeed);
    if (firework->isOver())
        firework = make_shared<Firework>();
    
    fwProg->bind();
    
    glUniformMatrix4fv(fwProg->getUniform("MV"), 1, GL_FALSE, MV->topMatrix().data());
    glUniformMatrix4fv(fwProg->getUniform("P"), 1, GL_FALSE, P->topMatrix().data());
    
    firework->draw(fwProg);
    
    fwProg->unbind();
    
    TwDraw();
	
	//////////////////////////////////////////////////////
	// Cleanup
	//////////////////////////////////////////////////////
	
	// Pop stacks
	MV->popMatrix();
	P->popMatrix();
	
	GLSL::checkError(GET_FILE_LINE);
    
    prevTime = time;
}

int main(int argc, char **argv)
{
    srand(NULL);
    
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
    
    // Set OpenGL Version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(1280, 720, "Fireworks Simulator", NULL, NULL);
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
	init();
    // Initialize AntTweakBar
    initAntTweakBar();
    
    
	// Loop until the user closes the window.
    while(!glfwWindowShouldClose(window)) {
        shared_ptr<int> visible = make_shared<int>(type == SHAPE);
        shared_ptr<int> notvisible = make_shared<int>(type != SHAPE);
        TwSetParam(myBar, "Browse...", "visible", TW_PARAM_INT32, 1, visible.get());
        TwSetParam(myBar, "Shape File", "visible", TW_PARAM_INT32, 1, visible.get());
        TwSetParam(myBar, "Explosion Duration", "visible", TW_PARAM_INT32, 1, notvisible.get());
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
