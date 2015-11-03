#pragma once 
#include "types.h"
#include <math.h>
#include "glm\glm.hpp"

#define PI 3.14159265359
#define perlinCurve(t) ( t * t * t * (t * (t * 6 - 15) + 10)); 

class MathHelper {
public:
	static inline GLfloat calcLength(point2df p) {
		return (sqrt(p.x*p.x + p.y*p.y));
	}
	static inline void normV(point2df &p)  {
		GLfloat l = calcLength(p);
		p.x = p.x / l;
		p.y = p.y / l;
	}

	static inline GLfloat dotProduct(point2df &p1, point2df &p2)  {
		return (p1.x*p2.x + p1.y * p2.y);
	}

	static inline  GLdouble linInterpolation(GLdouble start, GLdouble end, GLdouble t) {
		return start *(1.0f - t) + end*t;
	}

	static inline GLdouble  cosInterpolation(GLdouble start, GLdouble end, GLdouble t)  {
		GLdouble t2 = (1 - cos(t*PI)) / 2;
		return(start*(1 - t2) + end*t2);
		//return ((1.0+cos(t*PI/2.0))/2.0)*start+(1.0-(cos(t*PI/2.0))/2.0)*end;
	}

	static inline  GLdouble cubicInterpolation(GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3, GLdouble mu)  {
		GLdouble a0 = -0.5*v0 + 1.5*v1 - 1.5*v2 + 0.5*v3;
		GLdouble a1 = v0 - 2.5*v1 + 2 * v2 - 0.5*v3;
		GLdouble a2 = -0.5*v0 + 0.5*v2;
		GLdouble a3 = v1;
		return (a0 *mu*mu*mu) + (a1 *mu*mu) + (a2*mu) + a3;
	}

	static inline glm::vec3 calcTriangleNorm(vertexf a, vertexf b, vertexf c)
	{
		glm::vec3 edge1 = glm::vec3(b.x - a.x, b.y - a.y, b.z - a.z);
		glm::vec3 edge2 = glm::vec3(c.x - a.x, c.y - a.y, c.z - a.z);
		glm::vec3 crossv = glm::cross(edge1, edge2);
		return (glm::normalize(crossv));
	}
};