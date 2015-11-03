#include "Noise.h"


Noise::Noise()
{
	imageId = 0;
}

GLfloat temp[1000][1000];

void Noise::createGradients(GLint x, GLint y)
{
	GLuint degree = (rand() % 360);
	GLfloat radian = (degree * PI) / 180.0f;
	GLfloat xN = cosf(radian);
	GLfloat yN = sin(radian);
	point2df temp = { xN, yN };
	MathHelper::normV(temp);
	gradients.push_back(temp);
}

GLboolean Noise::MakeNoise(GLuint x, GLuint y, NoiseType type)
{
	GLuint seed = time(NULL);
	srand(seed);

	NoiseSettings settings;
	Grid grid{ x, y };
	settings.amplitude = 0.65f;
	settings.persistence = 0.65f;
	settings.frequency = 1.0f / (GLfloat)y;
	settings.grid = grid;
	settings.lucranity = 2.0f;
	settings.octaves = 6;
	settings.seed = seed;
	settings.random = rand();
	settings.size = 128.0f;
	settings.intType = InterpolationType::LINEAR;
	settings.intFnct = &MathHelper::cosInterpolation;

	currentImage.height = y;
	currentImage.width = x;
	//TODO CAN VERTICES ALSO BE NEGATIVE?
	std::vector<GLubyte> vertices;


	for (GLint ys = 0; ys < y; ys++)
	{
		for (GLint xs = 0; xs < x; xs++)
		{
			if (type == NoiseType::PERLIN)
			{
				createGradients(xs, ys);
			}
		}
	}

	if (type == NoiseType::PERLIN)
	{
		for (GLint ys = 0; ys < y; ys++)
		{
			for (GLint xs = 0; xs < x; xs++)
			{
				GLfloat newX = (GLfloat)xs * 0.00305f;
				GLfloat newY = (GLfloat)ys * 0.00305f;
				//vertices.push_back(255.0*perlinNoise(newX, newY, settings));
				vertices.push_back(128 + (128 * perlinNoiseOctave(newX, newY, settings)));
			}
		}
	}
	
	//Smooth it - two pass
	for (GLint ys = 0; ys < y; ys++)
	{
		for (GLint xs = 0; xs < x; xs++)
		{
			GLubyte vertex1 = vertices.at(ys*x + xs);
			GLint xs2 = (xs + 1) % x;
			GLubyte vertex2 = vertices.at(ys*x + xs2);
			GLdouble int1 = settings.intFnct(vertex1,vertex2,0.5);

			GLint ys2 = (ys + 1) % y;
			vertex1 = vertices.at(ys2*x + xs);
			vertex2 = vertices.at(ys2*x + xs2);
			GLdouble int2 = settings.intFnct(vertex1, vertex2, 0.5);
			
			vertices.at(ys*x + xs) = settings.intFnct(int1, int2, 0.5);
		}
	}
	for (GLint xs = 0; xs < x; xs++)
	{
		for (GLint ys = 0; ys < y; ys++)
		{
			GLubyte vertex1 = vertices.at(ys*x + xs);
			GLint xs2 = (xs + 1) % x;
			GLubyte vertex2 = vertices.at(ys*x + xs2);
			GLdouble int1 = settings.intFnct(vertex1, vertex2, 0.5);

			GLint ys2 = (ys + 1) % y;
			vertex1 = vertices.at(ys2*x + xs);
			vertex2 = vertices.at(ys2*x + xs2);
			GLdouble int2 = settings.intFnct(vertex1, vertex2, 0.5);

			vertices.at(ys*x + xs) = settings.intFnct(int1, int2, 0.5);
		}
	}

	currentImage.data = vertices;
	ilTexImage(x, y, 1, 1, IL_LUMINANCE, IL_UNSIGNED_BYTE, vertices.data());
	char buffer[200];
	sprintf_s(buffer, "%d.png", settings.random);
	ilSaveImage(buffer);
	
	return true;
}

GLboolean Noise::makeTexture() {
	glBindTexture(GL_TEXTURE_2D, NULL);
	glGenTextures(1, &currentImage.texture);
	glBindTexture(GL_TEXTURE_2D, currentImage.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, currentImage.width, currentImage.height, 0, GL_RED, GL_UNSIGNED_BYTE, currentImage.data.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, NULL);
	if (glGetError() != GL_NO_ERROR)
	{
		return false;
	}
	return true;
}

GLdouble Noise::hash(GLuint x, GLuint y)
{
	GLint temp = abs(sin(x*12.9898 + 78.233*y)) * 43785.5453;
	return (temp % 32768) / 32768.0;
}

GLdouble Noise::hash2(GLuint x, GLuint y)
{
	GLint n = x + y * 57;
	n = (n << 13) ^ n;
	return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

GLfloat Noise::perlinNoise(GLfloat x, GLfloat y, const NoiseSettings &settings)
{
	GLfloat noiseValue = 0.0f;
	//TODO also consider negative values
	GLint x0 = ((GLint)x % (settings.grid.xMax));
	GLint y0 = ((GLint)y % (settings.grid.yMax));
	GLint x1 = ((GLint)(x0 + 1) % (settings.grid.xMax));
	GLint y1 = ((GLint)(y0 + 1) % (settings.grid.yMax));
	GLfloat wX = x - (GLint)x;
	GLfloat wY = y - (GLint)y;
	GLfloat wX2 = wX - 1.0f;
	GLfloat wY2 = wY - 1.0f;
	GLfloat easeX = perlinCurve(wX);
	GLfloat easeY = perlinCurve(wY);

	GLfloat n0, n1, nPol1, nPol2;
	point2df a = { wX, wY };
	GLint gradPos = (y0 * ((settings.grid.xMax - 1) + ((x0 ) - 1)));
	
	n0 = MathHelper::dotProduct(a, gradients.at(gradPos));

	a = { wX2, wY };
	gradPos = (y0  * ((settings.grid.xMax - 1) + ((x1 ) - 1)));
	n1 = MathHelper::dotProduct(a, gradients.at(gradPos));
	nPol1 = settings.intFnct(n0, n1, easeX);

	a = { wX, wY2 };
	gradPos = (y1  * ((settings.grid.xMax - 1) + ((x0 ) - 1)));
	n0 = MathHelper::dotProduct(a, gradients.at(gradPos));

	a = { wX2, wY2 };
	gradPos = (y1  * ((settings.grid.xMax - 1) + ((x1 ) - 1)));
	n1 = MathHelper::dotProduct(a, gradients.at(gradPos));
	nPol2 = settings.intFnct(n0, n1, easeX);

	noiseValue = settings.intFnct(nPol1, nPol2, easeY);
	return noiseValue;
}


GLfloat Noise::perlinNoiseOctave(GLfloat x, GLfloat y, const NoiseSettings &settings)
{
	GLfloat value, frequence, amplitude,maxValue;
	value = 0.0f;
	frequence = 1.0f;
	amplitude = 1.0f;
	maxValue = 0.0f;
	for (GLuint i = 0; i < settings.octaves; i++)
	{
		value += perlinNoise(x*frequence, y*frequence, settings) * amplitude;
		frequence *= 2.0f;
		amplitude *= settings.persistence;
		maxValue += amplitude;
	}
	return value;
}

/*
GLdouble Noise::cosInterpolation(GLdouble start, GLdouble end, GLdouble t)
{
	GLdouble t2 = (1 - cos(t*PI)) / 2;
	return(start*(1 - t2) + end*t2);
	//return ((1.0+cos(t*PI/2.0))/2.0)*start+(1.0-(cos(t*PI/2.0))/2.0)*end;
}

GLfloat Noise::linInterpolation(GLfloat start, GLfloat end, GLfloat t)
{
	return start *(1.0f - t) + end*t;
}

GLdouble Noise::cubicInterpolation(GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3, GLdouble mu)
{
	GLdouble a0 = -0.5*v0 + 1.5*v1 - 1.5*v2 + 0.5*v3;
	GLdouble a1 = v0 - 2.5*v1 + 2 * v2 - 0.5*v3;
	GLdouble a2 = -0.5*v0 + 0.5*v2;
	GLdouble a3 = v1;
	return (a0 *mu*mu*mu) + (a1 *mu*mu) + (a2*mu) + a3;
}

GLdouble Noise::interpolate(GLdouble x, GLdouble y, const NoiseSettings& settings)
{
	GLdouble xNat = x - (GLint)x;
	GLdouble yNat = y - (GLint)y;
	GLdouble interpolatedValue = 0.0;
	/*if (settings.intType == InterpolationType::CUBIC)
	{
		GLdouble v1 = noise[((GLint)x -1 + 255) % 255][((GLint)y -1 + 255) % 255];
		GLdouble v2 = noise[((GLint)x + 255) % 255][((GLint)y -1 + 255) % 255];
		GLdouble v3 = noise[((GLint)x + 1+ 255) % 255][((GLint)y - 1 + 255) % 255];
		GLdouble v4 = noise[((GLint)x + 2+ 255) % 255][((GLint)y - 1 + 255) % 255];
		GLdouble t1 = cubicInterpolation(v1, v2, v3, v4, xNat);

		v1 = noise[((GLint)x + 1+ 255) % 255][((GLint)y + 255) % 255];
		v2 = noise[((GLint)x  + 255) % 255][((GLint)y + 255) % 255];
		v3 = noise[((GLint)x + 1 + 255) % 255][((GLint)y + 255) % 255];
		v4 = noise[((GLint)x + 2 + 255) % 255][((GLint)y + 255) % 255];
		GLdouble t2 = cubicInterpolation(v1, v2, v3, v4, xNat);

		v1 = noise[((GLint)x + 1 + 255) % 255][((GLint)y +1+ 255) % 255];
		v2 = noise[((GLint)x + 255) % 255][((GLint)y +1+ 255) % 255];
		v3 = noise[((GLint)x + 1 + 255) % 255][((GLint)y +1+ 255) % 255];
		v4 = noise[((GLint)x + 2 + 255) % 255][((GLint)y +1+ 255) % 255];
		GLdouble t3 = cubicInterpolation(v1, v2, v3, v4, xNat);

		v1 = noise[((GLint)x + 1 + 255) % 255][((GLint)y + 2 + 255) % 255];
		v2 = noise[((GLint)x + 255) % 255][((GLint)y + 2 + 255) % 255];
		v3 = noise[((GLint)x + 1 + 255) % 255][((GLint)y + 2 + 255) % 255];
		v4 = noise[((GLint)x + 2 + 255) % 255][((GLint)y + 2 + 255) % 255];
		GLdouble t4 = cubicInterpolation(v1, v2, v3, v4, xNat);

		interpolatedValue = cubicInterpolation(t1, t2, t3, t4, yNat);
		//TODO CUBIC INT
	}
	else if (settings.intType == InterpolationType::COSINE)
	{
		GLdouble v1 = noise[((GLint)x + 255) % 255][((GLint)y + 255) % 255];//hash((GLint)x, (GLint)y);
		GLdouble v2 = noise[((GLint)x + 255 + 1) % 255][((GLint)y + 255) % 255];//hash((GLint)x + 1, (GLint)y);
		GLdouble v3 = noise[((GLint)x + 255) % 255][((GLint)y + 255 + 1) % 255];//hash((GLint)x, (GLint)y + 1);
		GLdouble v4 = noise[((GLint)x + 255 + 1) % 255][((GLint)y + 255 + 1) % 255];//hash((GLint)x + 1, (GLint)y + 1);
		GLdouble interpolated1 = cosInterpolation(v1, v2, xNat);
		GLdouble interpolated2 = cosInterpolation(v3, v4, xNat);
		interpolatedValue = cosInterpolation(interpolated1, interpolated2, yNat);
	}
	else
	{
		GLdouble v1 = noise[((GLint)x  + 255) % 255][((GLint)y + 255) % 255];
		GLdouble v2 = noise[((GLint)x + 255 + 1) % 255][((GLint)y + 255) % 255];
		GLdouble v3 = noise[((GLint)x + 255) % 255][((GLint)y + 255 + 1) % 255];
		GLdouble v4 = noise[((GLint)x  + 1+255) % 255][((GLint)y + 255 + 1) % 255];
		GLdouble interpolated1 = linInterpolation(v1, v2, xNat);
		GLdouble interpolated2 = linInterpolation(v1, v3, xNat);
		interpolatedValue = linInterpolation(interpolated1, interpolated2, yNat);
	}
	return  interpolatedValue;
}

/*
GLdouble Noise::smoothNoise(GLdouble x, GLdouble y, GLdouble size, const NoiseSettings& settings)
{
	GLdouble value = 0.0, initialSize = size;
	while(size >= 1)
	{
		GLdouble newX = x / size;
		GLdouble newY = y / size;
		GLdouble xNat = newX - (GLint)newX;
		GLdouble yNat = newY - (GLint)newY;

		GLdouble v1 = temp[((GLint)newX + settings.grid.xMax) % settings.grid.xMax][((GLint)newY + settings.grid.yMax) % settings.grid.yMax];
		GLdouble v2 = temp[((GLint)newX + settings.grid.xMax + 1) % settings.grid.xMax][((GLint)newY + settings.grid.yMax) % settings.grid.yMax];
		


		//GLdouble v1 = noise.at(((GLint)newY % settings.grid.yMax)*settings.grid.xMax + ((GLint)newX % settings.grid.xMax));
		//GLdouble v2 = noise.at(((GLint)newY % settings.grid.yMax)*settings.grid.xMax + (((GLint)newX + 1) % settings.grid.xMax));
		GLdouble interpolated1 = settings.intFnct(v1,v2,xNat);
		v1 = temp[((GLint)newX + settings.grid.xMax) % settings.grid.xMax][((GLint)newY + 1 + settings.grid.yMax) % settings.grid.yMax];
		v2 = temp[((GLint)newX + 1 + settings.grid.xMax + 1) % settings.grid.xMax][((GLint)newY + 1 + settings.grid.yMax) % settings.grid.yMax];
		//v1 = noise.at((((GLint)newY + 1) % settings.grid.yMax)*settings.grid.xMax + ((GLint)newX % settings.grid.xMax));
		//v2 = noise.at((((GLint)newY + 1) % settings.grid.yMax)*settings.grid.xMax + (((GLint)newX + 1) % settings.grid.xMax));
		GLdouble interpolated2 = settings.intFnct(v1, v2, xNat);
		
		value += settings.intFnct(interpolated1, interpolated2, yNat)*size;
		size /= 2.0;
	}
	return (128.0 * value/initialSize );
}
*/

/*if (type == NoiseType::SMOOTH)
{
for (int ys = 0; ys < y; ys++)
{
for (int xs = 0; xs < x; xs++)
{
GLfloat newX = (GLfloat)xs * 0.0135f;
GLfloat newY = (GLfloat)ys * 0.0135f;
vertices.push_back(smoothNoise(newX, newY, settings.size, settings));
//vertices.push_back(255.0*perlinNoise(xs, ys, settings));
}
}
}
else */


/*ilGenImages(1, &imageId);
ilBindImage(imageId);
char buffer[200];
sprintf(buffer, "%d.png", settings.random);
if (IL_TRUE != ilLoadImage(buffer))
{
printf("ERROR WHILE LOADING IMAGE");
return false;
}
if (IL_TRUE != ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE))
{
printf("ERROR WHILE CONVERTING IMAGE");
return false;
}
currentImage.data = ilGetData();
currentImage.height = ilGetInteger(IL_IMAGE_HEIGHT);
currentImage.width = ilGetInteger(IL_IMAGE_WIDTH);

//ilSaveImage("test3.png");
if (!makeTexture())
{
printf("ERROR WHILE CREATING TEXTURE: %s\n", gluErrorString(error));
return false;
}*/