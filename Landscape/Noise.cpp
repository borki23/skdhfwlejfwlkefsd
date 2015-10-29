#include "Noise.h"

Noise::Noise()
{
	imageId = 0;
}

GLfloat noise[255][255];

void Noise::createGradients(GLint x, GLint y)
{
	GLfloat xUnit = (rand() % 100) / 100.0f;
	GLfloat yUnit = (rand() % 100) / 100.0f;
	if (rand() % 100 > 50)
	{
		xUnit *= -1.0;
	}
	if (rand() % 100 > 50)
	{
		yUnit *= -1.0;
	}
	point2df temp = { xUnit, yUnit };
	//MathHelper::normV(temp);
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
	settings.octaves = 12;
	settings.seed = seed;
	settings.random = rand();
	settings.size = 128.0f;
	settings.intType = InterpolationType::COSINE;

	currentImage.height = y;
	currentImage.width = x;
	//TODO CAN VERTICES ALSO BE NEGATIVE?
	std::vector<GLubyte> vertices;

	
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
	for (int ys = 0; ys < 255; ys++)
	{
		for (int xs = 0; xs < 255; xs++)
		{
			noise[ys][xs] = (rand() % 32768) / 32768.0;
			if (type == NoiseType::PERLIN)
			{
				createGradients(xs, ys);
			}
		}
	}

	if (type == NoiseType::PERLIN)
	{
		for (int ys = 0; ys < y; ys++)
		{
			for (int xs = 0; xs < x; xs++)
			{
				vertices.push_back(smoothNoise(xs, ys, settings.size, settings));
				//vertices.push_back(255.0*perlinNoise(xs, ys, settings));
			}
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

GLfloat Noise::perlinNoise(GLuint x, GLuint y, const NoiseSettings &settings)
{
	GLfloat noiseValue = 0.0f;
	GLfloat startFrequency = settings.frequency;
	GLfloat startAmplitude = settings.amplitude;
	for (GLuint oct = 0; oct < settings.octaves; oct++)
	{
		//TODO GLdouble
		noiseValue += interpolate((GLfloat)x * startFrequency, (GLfloat)y * startFrequency, settings)*startAmplitude;//smoothTest((GLfloat)x * startFrequency, (GLfloat)y * startFrequency)*startAmplitude;
		startFrequency *= settings.lucranity;
		startAmplitude *= settings.persistence;
	}
	return noiseValue;
}

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
	if (settings.intType == InterpolationType::CUBIC)
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

/*GLdouble Noise::smoothTest(GLdouble x, GLdouble y)
{
	GLdouble xNat = x - (GLint)x;
	GLdouble yNat = y - (GLint)y;

	int x1 = ((GLint)x + 255) % 255;
	int y1 = ((GLint)y + 255) % 255;

	int x2 = ((GLint)x + 255 - 1) % 255;
	int y2 = ((GLint)y + 255 - 1) % 255;

	GLdouble value = 0.0;
	value += xNat       * yNat       *noise[x1][y1];// hash(x1,y1);
	value += xNat       * (1 - yNat) * noise[x1][y2];//hash(x1, y2);
	value += (1 - xNat) * yNat       *noise[x2][y1];//hash(x2, y1);
	value += (1 - xNat) * (1 - yNat) * noise[x2][y2];//hash(x2, y2);

	return value;
}*/

GLdouble Noise::smoothNoise(GLdouble x, GLdouble y, GLdouble size, const NoiseSettings& settings)
{
	GLdouble value = 0.0, initialSize = size;
	while(size >= 1)
	{
		value += interpolate(x / size, y / size, settings)*size;
		size /= 2.0;
	}
	return (128.0 * value/initialSize );
}