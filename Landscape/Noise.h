#ifndef __PERLIN_NOISE_H__
#define __PERLIN_NOISE_H__
#include "IL\il.h"
#include "GL\freeglut.h"
#include <time.h>
#include <random>
#include "types.h"
#include "MathHelper.h"    

struct NoiseImage {
	GLint width;
	GLint height;
	//GLubyte* data;
	//TODO CAN VERTICES ALSO BE NEGATIVE?
	std::vector<GLubyte> data;
	GLuint texture;
};

struct Grid{
	GLuint yMax;
	GLuint xMax;
};

enum NoiseType {
	PERLIN,
	//SMOOTH,
	FBM
};

enum InterpolationType {
	LINEAR,
	COSINE,
	CUBIC
};

enum TerrainType{
	FIELD,
	MOUNTAIN,
	VALLEY
};

struct NoiseSettings{
	GLfloat amplitude;
	GLfloat lucranity;
	GLfloat frequency;
	GLfloat persistence;
	GLint seed;
	Grid grid;
	GLuint octaves;
	GLint random;
	InterpolationType intType;
	GLfloat size;
	//cubic int not supportet :]
	GLdouble(*intFnct) (GLdouble, GLdouble, GLdouble);
	GLdouble(*hashFnct) (GLdouble, GLdouble);
};

class Noise{
	public:
		Noise();
		GLboolean MakeNoise(GLuint x, GLuint y, NoiseType type);
		NoiseImage getCurrentNoiseImage() {
			return currentImage;
		}

	private:
		ILuint imageId;
		NoiseImage currentImage;
		std::vector<point2df> gradients;
		std::vector<GLfloat> noise;
		void createGradients(GLint x, GLint y);
		GLfloat dotGridGrad(GLint gridX, GLint gridY, GLfloat pX, GLfloat pY, const NoiseSettings &settings);

		GLboolean makeTexture();
		GLdouble interpolate(GLdouble x, GLdouble y, const NoiseSettings& settings);

		/*GLfloat linInterpolation(GLfloat start, GLfloat end, GLfloat t);
		GLdouble cosInterpolation(GLdouble start, GLdouble end, GLdouble t);
		GLdouble cubicInterpolation(GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3, GLdouble mu);
		*/
		//GLdouble smoothTest(GLdouble x, GLdouble y);
		//http://lodev.org/cgtutor/randomnoise.html#Smooth_Noise_
		GLdouble smoothNoise(GLdouble x, GLdouble y, GLdouble size, const NoiseSettings& settings);
		//
		GLfloat perlinNoise(GLfloat x, GLfloat y, const NoiseSettings &settings);
		GLfloat perlinNoiseOctave(GLfloat x, GLfloat y, const NoiseSettings &settings);

		GLdouble hash(GLuint x, GLuint y);
		GLdouble hash2(GLuint x, GLuint y);

		
		
};

#endif;