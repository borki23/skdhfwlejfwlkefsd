#include "Landscape.h"
#include <stdio.h>

Landscape::Landscape(){
	wireFrame = false;
}

GLboolean Landscape::draw()
{
	glm::vec3 currentVertex;
	glm::vec3 lastVertex;

	glColor3f(0.0f,1.0f,0.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_POINTS, info.indices.size(), GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	/*glBegin(GL_QUADS);
	glColor3f(0.f, 1.f, 1.f);
		glVertex3f(-50.f, 0, -500.0f);
		glVertex3f(50.f, 0, -500.f);
		glVertex3f(50.f, 0, 500.0f);
		glVertex3f(-50.f, 0, 500.f);
	glEnd(); 
	//landScapeShader.updateModelView();
	/*glBegin(GL_QUADS);
	glColor3f(0.f, 1.f, 1.f);
	glVertex3f(-50.f, 0, -500.0f);
	glVertex3f(-50.f,50.0f, -500.f);
	glVertex3f(50.f, 50.0, -500.0f);
	glVertex3f(50.f, 0, -500.f);
	glEnd();*/
	/*if (!wireFrame)
	{
		glBegin(GL_TRIANGLE_STRIP);
	}
	for (unsigned int z = 0; z < info.height - 1; z++) {
		if (wireFrame)
		{
			glBegin(GL_LINES);
			glColor3f(0.0f, 1.0f, 0.0f);
		}
		lastVertex = glm::vec3(0, 0, z*scale);
		for (unsigned int x = 0; x < info.width; x++) {
			GLubyte pixel = info.data[x + (z* info.width)];
			currentVertex.x = x * scale;
			currentVertex.z = z * scale;
			currentVertex.y = pixel * scale;
			if (!wireFrame)
			{
				glColor3f(pixel / 230.0f, pixel / 230.0f, pixel / 230.0f);
				glVertex3f(currentVertex.x, currentVertex.y, currentVertex.z);
				glVertex3f(currentVertex.x, info.data[(x + (z + 1)* info.width)] * scale, currentVertex.z + (1 * scale));
			}
			else
			{
				glVertex3f(lastVertex.x, lastVertex.y, lastVertex.z);
				glVertex3f(currentVertex.x, currentVertex.y, currentVertex.z);
				lastVertex = currentVertex;
			}
		}
		if (wireFrame)
		{
			glEnd();
		}else
		{
			glVertex3f(currentVertex.x, currentVertex.y, currentVertex.z *scale);
			glVertex3f(0, info.data[(z* info.width)] * scale, currentVertex.z + (1 * scale));
		}

		if (glGetError() != GL_NO_ERROR) 
		{
			printf((char*)gluErrorString(glGetError()));
			return false;
		}
	}
	if (!wireFrame)
	{
		glEnd();
	}
	//landScapeShader.unbind();
	if (glGetError() != GL_NO_ERROR)
	{
		printf((char*)gluErrorString(glGetError()));
		return false;
	}*/
	return true;
}

GLboolean Landscape::update(GLfloat time)
{

	return true;
}

int Landscape::makeLandscape()
{
	vertexf currentVertex;
	vertexf lastVertex;

	GLuint currentIndex = 0;
	for (unsigned int z = 0; z < info.width; z++)
	{
		lastVertex = { 0, 0, z*scale };
		for (unsigned int x = 0; x < info.width; x++) 
		{
			currentIndex = x + (z* info.width);
			info.indices.push_back(currentIndex);	
			GLubyte pixel = info.data[currentIndex];
			currentVertex.x = x * scale;
			currentVertex.z = z * scale;
			currentVertex.y = pixel * scale;
			//info.vertexes.push_back(currentVertex.x);
			//info.vertexes.push_back(currentVertex.y);
			//info.vertexes.push_back(currentVertex.z);
			info.svertexes.push_back(currentVertex);

			lastVertex = currentVertex;
			
		}
	}
	if (glGetError() != GL_NO_ERROR)
	{
		printf( (char*)gluErrorString(glGetError()));
		//TODO WHY?
	}

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//glBufferData(GL_ARRAY_BUFFER, info.vertexes.size()*sizeof(GLfloat), info.vertexes.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, info.svertexes.size()*sizeof(vertexf), info.svertexes.data(), GL_STATIC_DRAW);
	
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indices.size()*sizeof(GLuint), info.indices.data(), GL_STATIC_DRAW);
	glBindBuffer(0, NULL);
	readyToDraw = true;
	return true;
}

Landscape::Landscape(float scale, const LandscapeInfo &info, const std::string& identifier) {
	this->scale = scale;
	this->info = info;
	wireFrame = false;

	updateIdentifier = identifier;
	drawIdentifier = identifier;

	standardShader.SetShader("landscape_vertex.glsl", GL_VERTEX_SHADER);
	standardShader.SetShader("landscape_fragment.glsl", GL_FRAGMENT_SHADER);
	standardShader.initProgram();
	standardShader.bind();

	myShader.push_back(standardShader);
}
