#pragma once 
#include "types.h"
#include <math.h>

class MathHelper {
public:
	static GLfloat calcLength(point2df p) {
		return (sqrt(p.x*p.x + p.y*p.y));
	}
	static void normV(point2df &p) {
		GLfloat l = calcLength(p);
		p.x = p.x / l;
		p.y = p.y / l;
	}
};