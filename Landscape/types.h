#pragma once

#include "GL\freeglut.h"

struct vertexi {
	GLint x;
	GLint y;
	GLint z;
};

struct vertexf {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	//GLfloat nx;
	//GLfloat ny;
	//GLfloat nz;
};


struct point2di
{
	GLint x;
	GLint y;
};

struct point2df
{
	GLfloat x;
	GLfloat y;
};