#pragma once

#include "GL\glew.h"
#include "GL\freeglut.h"
#include "glm\glm.hpp"
#include "Shader.h"
#include <vector>
#include "Shader.h"
#include "Interfaces.h"
#include "types.h"

struct LandscapeInfo 
{
	GLuint width;
	GLuint height;
	std::vector<GLubyte> data;
	std::vector<GLfloat> vertexes;
	std::vector<vertexf> svertexes;
	std::vector<GLint> indices;
};

class Landscape : public Drawable, public Updateable
{
	private:
		GLfloat scale;
		LandscapeInfo info;
		GLboolean wireFrame = false;

		Shader standardShader;

		GLboolean surfaceChanged;
		//GLuint currentDrawMode;
		GLuint indexBuffer;
		GLuint vertexBuffer;
	public:
		Landscape();
		Landscape(GLfloat scale, const LandscapeInfo &info, const std::string& identifier);
		~Landscape();
		 GLboolean draw();
		 GLboolean update(GLfloat time);
 
		void updateSurface();

		GLint makeLandscape();

		inline void toogleWireFrame()
		{
			wireFrame = !wireFrame;
		}

};