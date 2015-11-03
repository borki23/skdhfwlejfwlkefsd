#pragma once

#include "GL\glew.h"
#include "GL\freeglut.h"
#include "glm\glm.hpp"
#include "Shader.h"
#include <vector>
#include "Shader.h"
#include "Interfaces.h"
#include "types.h"
#include "MathHelper.h"

struct LandscapeInfo 
{
	GLuint width;
	GLuint height;
	std::vector<GLubyte> data;
	std::vector<GLfloat> vertexes;
	std::vector<vertexf> svertexes;
	std::vector<GLint> indices;
	std::vector<glm::vec3> normals;
};

class Landscape : public Drawable, public Updateable
{
	private:
		GLfloat scale;
		LandscapeInfo info;
		GLboolean wireFrame = false;

		Shader standardShader;
		Shader clearShader;

		GLboolean surfaceChanged;
		//GLuint currentDrawMode;
		//TODO OUTSOURCE IN PROCEDURAL.h
		GLuint indexBuffer;
		GLuint vertexBuffer;
		GLuint normBuffer;
		GLint bTerrainMode;
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