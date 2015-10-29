#pragma once

#include "GL\freeglut.h"
#include "glm\glm.hpp"
#include <vector>
#include "types.h"

class Quadtree {
	private:
		Quadtree* nw;
		Quadtree* ne;
		Quadtree* sw;
		Quadtree* se;

		vertexf center;


		std::vector<vertexf> data;
	public:
		Quadtree();
		~Quadtree();
		GLboolean subdivide();
		GLboolean addData();

		GLfloat distanceToPoint(vertexf point) const;
};