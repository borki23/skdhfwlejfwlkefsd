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
	glUniform1f(bTerrainMode,1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	//glDrawElements(GL_POINTS, info.indices.size(), GL_UNSIGNED_INT, NULL);
	glDrawElements(GL_TRIANGLE_STRIP, info.indices.size(), GL_UNSIGNED_INT, NULL);

	
	glDisableClientState(GL_VERTEX_ARRAY);
	glUniform1f(bTerrainMode, 0.0f);
	glBegin(GL_LINES);
	glLineWidth(15.0f);
	for (unsigned int z = 0; z < info.height/2 ; z++){
		for (unsigned int x = 0; x < info.width ; x++){
			glm::vec3 normal =  info.normals.at(z*info.height + x);
			vertexf vert = info.svertexes.at(z*info.height + x);
			glVertex3f(vert.x, vert.y, vert.z);
			//glVertex3f(vert.x, vert.y+10.0f, vert.z);
			glVertex3f(vert.x + normal.x*2.0f, vert.y+normal.y*2.0f, vert.z+normal.z*2.0f);
			//glVertex3f(vert.x , vert.y + 10.0f, vert.z);
		}
	}
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

GLboolean Landscape::update(GLfloat time){

	return true;
}

int Landscape::makeLandscape(){
	vertexf currentVertex;
	vertexf lastVertex;
	vertexf tempVertex;
	GLuint currentIndex = 0;
	GLuint xCount = 1;
	GLubyte pixel = 0;
	GLuint temp;

	if (!wireFrame)
	{
	//Count = 2;
	}
	//TODO MAKE INDICES ONLY
	for (unsigned int z = 0; z < info.height - 1; z++){
		for (unsigned int x = 0; x < info.width - 1 ; x++){
			/*if (wireFrame)
			{
				currentIndex = x + (z* info.width);
				info.indices.push_back(currentIndex);

				//info.vertexes.push_back(currentVertex.x);
				//info.vertexes.push_back(currentVertex.y);
				//info.vertexes.push_back(currentVertex.z);
			}
			else
			{
				currentIndex = x + (z* info.width);
				info.indices.push_back(currentIndex);
			}*/

			temp = x + z*info.height;
			pixel = info.data[temp];
			currentVertex = { x*scale, pixel* scale, z*scale };
			info.svertexes.push_back(currentVertex);
			info.indices.push_back(currentIndex);
			currentIndex++;

			temp = x + (z + 1) * (info.height);
			pixel = info.data[temp];
			lastVertex = { x*scale, pixel * scale, (z + 1)*scale };
			info.svertexes.push_back(lastVertex);
			info.indices.push_back(currentIndex);
			currentIndex++;

			//lastVertex = currentVertex;
		}
		//degenerate
		info.svertexes.push_back(lastVertex);
		info.indices.push_back(currentIndex);
		currentIndex++;

		temp = (z + 1) * (info.height);
		pixel = info.data[temp];
		currentVertex = { 0, pixel * scale, (z + 1)*scale };
		info.svertexes.push_back(currentVertex);
		info.indices.push_back(currentIndex);
		currentIndex++;
	}
	//Calcnormals
	//TODO should be in Procedural.h
	vertexf vertex1, vertex2;
	
	//TODO MAKE VERTEXESON THE LEFT ASWELL
	for (unsigned int z = 0; z < info.height - 1; z++){
		for (unsigned int x = 0; x < info.width - 1; x++){
			vertexf currVertex = info.svertexes.at(z*info.height +x);
			if (z == 0 && x == 0)
			{
				vertex1 = info.svertexes.at((z + 1)*info.height);
				vertex2 = info.svertexes.at(1);
				glm::vec3 triangleNorm = MathHelper::calcTriangleNorm(currVertex, vertex1, vertex2);
				info.normals.push_back(glm::normalize(triangleNorm));
			}else if (z == 0 && x != 0)
			{
				vertex1 = info.svertexes.at(x-1);
				vertex2 = info.svertexes.at((z+1)*info.height + ( x-1));
				glm::vec3 triangleNorm1 = MathHelper::calcTriangleNorm(currVertex, vertex1, vertex2);

				vertex1 = info.svertexes.at(x - 1);
				vertex2 = info.svertexes.at((z + 1)*info.height + (x));
				glm::vec3 triangleNorm2 = MathHelper::calcTriangleNorm(currVertex, vertex1, vertex2);

				vertex1 = info.svertexes.at(x + 1);
				vertex2 = info.svertexes.at((z + 1)*info.height + (x));
				glm::vec3 triangleNorm3 = MathHelper::calcTriangleNorm(currVertex, vertex1, vertex2);
				
				glm::vec3 sum = triangleNorm3 + triangleNorm2 + triangleNorm1;
				info.normals.push_back(glm::normalize(sum));
			}
			else if (z != 0 && x != 0)
			{
				vertex1 = info.svertexes.at(x - 1);
				vertex2 = info.svertexes.at((z + 1)*info.height + (x - 1));
				glm::vec3 triangleNorm1 = MathHelper::calcTriangleNorm(currVertex, vertex1, vertex2);

				vertex1 = info.svertexes.at(x - 1);
				vertex2 = info.svertexes.at((z + 1)*info.height + (x));
				glm::vec3 triangleNorm2 = MathHelper::calcTriangleNorm(currVertex, vertex1, vertex2);

				vertex1 = info.svertexes.at(x + 1);
				vertex2 = info.svertexes.at((z + 1)*info.height + (x));
				glm::vec3 triangleNorm3 = MathHelper::calcTriangleNorm(currVertex, vertex1, vertex2);

				vertex1 = info.svertexes.at(x+1);
				vertex2 = info.svertexes.at((z + 1)*info.height + (x));
				glm::vec3 triangleNorm4 = MathHelper::calcTriangleNorm(currVertex, vertex1, vertex2);

				vertex1 = info.svertexes.at(x + 1);
				vertex2 = info.svertexes.at((z - 1)*info.height + (x+1));
				glm::vec3 triangleNorm5 = MathHelper::calcTriangleNorm(currVertex, vertex1, vertex2);

				vertex1 = info.svertexes.at((z - 1)*info.height + x);
				vertex2 = info.svertexes.at((z - 1)*info.height + (x + 1));
				glm::vec3 triangleNorm6 = MathHelper::calcTriangleNorm(currVertex, vertex1, vertex2);

				glm::vec3 sum = triangleNorm3 + triangleNorm2 + triangleNorm1 + triangleNorm6 + triangleNorm5 + triangleNorm4;
				info.normals.push_back(glm::normalize(sum));
			}
		}
	}

	if (glGetError() != GL_NO_ERROR)
	{
		printf( (char*)gluErrorString(glGetError()));
		//TODO WHY?
	}

	glGenBuffers(1, &normBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normBuffer);
	//glBufferData(GL_ARRAY_BUFFER, info.vertexes.size()*sizeof(GLfloat), info.vertexes.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, info.normals.size()*sizeof(glm::vec3), info.normals.data(), GL_STATIC_DRAW);
	

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

	//clearShader.SetShader("clearShader_vertex.glsl", GL_VERTEX_SHADER);
	//clearShader.SetShader("clearShader_fragment.glsl", GL_FRAGMENT_SHADER);
	//clearShader.initProgram();

	standardShader.SetShader("landscape_vertex.glsl", GL_VERTEX_SHADER);
	standardShader.SetShader("landscape_fragment.glsl", GL_FRAGMENT_SHADER);
	standardShader.initProgram();
	bTerrainMode = glGetUniformLocation(standardShader.getProgramID(), "bTerrainMode");
	if (bTerrainMode == -1)
	{
		printf("Cannot find attribute: %s", "bTerrainMode");
		//printProgramError(m_nProgramID);
		//return ERROR_CANNOT_FIND_UNIFORM;
	}
	standardShader.bind();
	
	//myShader.push_back(clearShader);
	myShader.push_back(standardShader);
}
