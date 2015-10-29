//#define DEBUG_ON

#include "GL\glew.h"
#include "GL\freeglut.h"
#include <stdio.h>
#include "Shader.h"
#include <iostream>
#include "Noise.h"
#include "types.h"
#include "Landscape.h"
#include "camera.h"
#include "Interfaces.h"

#define SPACE 32
#define CTRL 256
#define LEFTMOUSEBTN 2

const unsigned int WIN_XPOS = 250;
const unsigned int WIN_YPOS = 250;
extern const unsigned int WIN_WIDTH = 750;
extern const unsigned int WIN_HEIGHT = 750;

Camera cam(glm::perspective(45.0f, WIN_WIDTH / WIN_HEIGHT*1.0f, 1.0f, 10000.0f));
//TODO outsource in input class
GLfloat LookPitchDegree = 0.0f;
GLfloat LookYawDegree = 10.0f;
GLfloat lastPitchPos = 0.0f;
GLfloat lastYawPos = 0.0f;
GLfloat moveY = 0.0f;
GLfloat moveX = 0.0f;
GLfloat moveZ = -10.0f;
GLfloat currMovSpeed = 0.1f;
GLfloat currLookSpeed = 0.3f;
bool look = false;
bool move = false;
bool firstMouse = true;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

bool keyActive[257];
bool toggleKeyActive[257];

std::vector<Drawable*> drawables;
std::vector<Updateable*> updatables;

GLushort oldKey;
bool setupWorld();
void moveCamera();

void renderScene()
{
	float currentTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
	if (GetKeyState(VK_LCONTROL) & 0x8000)
	{
		keyActive[CTRL] = true;
	}
	else 
	{
	     keyActive[CTRL] = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	moveCamera();
	cam.look();

	#ifdef DEBUG_ON
		printf("%f\n", currentTime);
		printf("%f-%f-%f | %f-%f-%f \n", cam.getXPos(), cam.getYPos(), cam.getZPos(), cam.getXFront(), cam.getYFront(), cam.getZFront());
	#endif

	for (Updateable* updatable : updatables)
	{
		if (updatable->canBeUpdated())
		{
			updatable->updateShaderTime(currentTime);
			updatable->updateShadersModelView(cam.getModelView());
		}
	}
	for (Drawable* drawable : drawables)
	{
		if (drawable->canBeDrawn())
		{
			drawable->draw();
		}
	}
	//Update screen
	glutSwapBuffers();
}

void changeSize(int w, int h)
{
	/*glViewport(0.f, 0.f, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(0.0, WIN_WIDTH, WIN_HEIGHT, 0.0, 10.0, -10.0);
	gluPerspective(45.0f, w / h, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
}

void keysPressed(unsigned char id, int x, int y)
{
	
	keyActive[id] = true;
	toggleKeyActive[id] = !toggleKeyActive[id];

	if (id == 't')
	{
		//landscape->toogleWireFrame();
	}
}

void keysReleased(unsigned char id, int x, int y)
{
	
	keyActive[id] = false;
}

void moveCamera() 
{
	if (keyActive['w'])
	{
		cam.moveCamera(Direction::Forward, currMovSpeed * deltaTime);
	}
	if (keyActive['s'])
	{
		cam.moveCamera(Direction::Backward, currMovSpeed * deltaTime);
	}
	if (keyActive[SPACE])
	{
		cam.moveCamera(Direction::Up, currMovSpeed * deltaTime);
	}
	if (keyActive[CTRL])
	{
		cam.moveCamera(Direction::Down, currMovSpeed * deltaTime);
	}
	if (keyActive['a'])
	{
		cam.moveCamera(Direction::Right, currMovSpeed * deltaTime);
	}
	if (keyActive['d'])
	{
		cam.moveCamera(Direction::Left, currMovSpeed * deltaTime);
	}
	if (toggleKeyActive['t'])
	{
		//landscape->toogleWireFrame();
	}
}

void processMouseButtons(int a, int b, int c, int d)
{
	if (a == LEFTMOUSEBTN) {
		look = b;
		if (firstMouse) {
			lastPitchPos = d;
			lastYawPos = c;
			firstMouse = false;
		}
	}
}

void processMouseMotion(int a, int b)
{
	if (look)
	{
		LookPitchDegree += (lastPitchPos - b) * currLookSpeed;
		lastPitchPos = b;
		LookYawDegree -= (lastYawPos - a) * currLookSpeed;
		lastYawPos = a;
		cam.rotateCamera(LookYawDegree, LookPitchDegree);
	}
}

void initGL()
{
	glewInit();
	glViewport(0.f, 0.f, WIN_WIDTH, WIN_HEIGHT);

	glClearColor(0, 0, 0, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	
	if (glGetError() != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(glGetError()));
	}
}



int main(int argc, char ** args)
{
	glutInit(&argc,args);

	glutInitContextVersion(3,0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	//glutInitContextFlags(GLUT_DEBUG);
	
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition(WIN_XPOS, WIN_YPOS);

	glutCreateWindow("Landscape...");

	//Handler functions
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutKeyboardFunc(keysPressed);
	glutKeyboardUpFunc(keysReleased);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	initGL();
	ilInit();
	ilClearColour(255, 255, 255, 000);

	if (setupWorld())
	{
		printf("Cannot create world");
		return 1;
	}

	glutMainLoop();
	return 0;
}

bool setupWorld()
{
	Noise noise;
	LandscapeInfo landscapeInfo;
	Landscape* landscape;

	//Make start perlin noise 
	noise.MakeNoise(255, 255, NoiseType::PERLIN);
	landscapeInfo.height = noise.getCurrentNoiseImage().height;
	landscapeInfo.width = noise.getCurrentNoiseImage().width;
	landscapeInfo.data = noise.getCurrentNoiseImage().data;
	landscape = new Landscape(0.5f, landscapeInfo, "landscape");
	if (!landscape->makeLandscape())
	{
		printf("Error while creating landscape...");
		return 1;
	}
	//init shaders
	landscape->updateShadersModelView(cam.getModelView());
	landscape->updateShadersPerspective(cam.getPerspective());
	drawables.push_back(landscape);
	updatables.push_back(landscape);
	return 0;
}
