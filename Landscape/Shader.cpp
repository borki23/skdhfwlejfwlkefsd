#include "Shader.h"
#include <fstream>
#include <iostream>


Shader::Shader()
{
	//VIew* modell matrix.
	m_currentModelViewMatrix = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)) * glm::mat4(1.0f);
	m_currentPerspectiveMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 1.0f, 10.0f);
}

Shader::Shader(glm::mat4 &gModelViewMatrix, glm::mat4 &gPerspectiveMatrix)
{
	m_currentModelViewMatrix = gModelViewMatrix;
	m_currentPerspectiveMatrix = gPerspectiveMatrix;
}

Shader::~Shader()
{

}

GLuint Shader::initProgram()
{
	if (m_nProgramID)
	{
		glDeleteProgram(m_nProgramID);
	}
	if (m_nFragmentShaderID == 0  || m_nVertexShaderID == 0)
	{
		return ERROR_SHADER_NOT_COMPILED;
	}
	m_nProgramID = glCreateProgram();
	
	glAttachShader(m_nProgramID, m_nVertexShaderID);
	glAttachShader(m_nProgramID, m_nFragmentShaderID);
	glLinkProgram(m_nProgramID);

	GLint nLinkSuccess = GL_FALSE;
	glGetProgramiv(m_nProgramID, GL_LINK_STATUS, &nLinkSuccess);
	if ((glGetError() != GL_NO_ERROR) || (nLinkSuccess != GL_TRUE))
	{
		int nLogLength = 0;
		int nMaxLength = 0;

		glGetProgramiv(m_nProgramID, GL_INFO_LOG_LENGTH, &nMaxLength);
		char* cInfo = new char[nMaxLength];
		glGetProgramInfoLog(m_nProgramID, nMaxLength, &nLogLength, cInfo);
		std::cout << cInfo << std::endl;
		delete[] cInfo;
		glDeleteShader(m_nVertexShaderID);
		glDeleteShader(m_nFragmentShaderID);
		glDeleteProgram(m_nProgramID);
		return ERROR_CANNOT_LINK_PROGRAM;
	}
	glDeleteShader(m_nVertexShaderID);
	glDeleteShader(m_nFragmentShaderID);

	m_nModelViewMatrixPos = glGetUniformLocation(m_nProgramID, MODELVIEWNAME);
	if (m_nModelViewMatrixPos == -1)
	{
		printf("Cannot find uniform: %s", MODELVIEWNAME);
		return ERROR_CANNOT_FIND_UNIFORM;
	}

	m_nPerspectiveMatrixPos = glGetUniformLocation(m_nProgramID, PROJECTIONNAME);
	if (m_nModelViewMatrixPos == -1)
	{
		printf("Cannot find uniform: %s", PROJECTIONNAME);
		return ERROR_CANNOT_FIND_UNIFORM;
	}

	m_nCurrentTimePos = glGetUniformLocation(m_nProgramID, TIMENAME);
	if (m_nCurrentTimePos == -1)
	{
		printf("Cannot find uniform: %s", TIMENAME);
		return ERROR_CANNOT_FIND_UNIFORM;
	}

	glUniformMatrix4fv(m_nModelViewMatrixPos, 1, GL_FALSE, glm::value_ptr(m_currentModelViewMatrix));
	glUniformMatrix4fv(m_nPerspectiveMatrixPos, 1, GL_FALSE, glm::value_ptr(m_currentPerspectiveMatrix));
	
	return 0;
}

GLuint Shader::SetShader(const std::string & sShader, GLenum eType)
{
	std::ifstream fShaderSource(sShader.c_str());
	const GLchar* shaderSource;
	GLuint glLastShader;
	if (fShaderSource)
	{
		switch (eType)
		{
			case GL_VERTEX_SHADER: //taken from lazyfoo tutorials =]
									m_sCurrentVertexShader.assign(std::istreambuf_iterator<char>(fShaderSource), std::istreambuf_iterator<char>());
									m_nVertexShaderID = glCreateShader(eType);
									shaderSource = m_sCurrentVertexShader.c_str();
									glShaderSource(m_nVertexShaderID, 1, &shaderSource, NULL);
									glCompileShader(m_nVertexShaderID);
									glLastShader = m_nVertexShaderID;
									
									break;
			case GL_FRAGMENT_SHADER: //taken from lazyfoo tutorials =]
									m_sCurrentFragmentShader.assign(std::istreambuf_iterator<char>(fShaderSource), std::istreambuf_iterator<char>());
									m_nFragmentShaderID = glCreateShader(eType);
									shaderSource = m_sCurrentFragmentShader.c_str();
									glShaderSource(m_nFragmentShaderID, 1, &shaderSource, NULL);
									glCompileShader(m_nFragmentShaderID);
									glLastShader = m_nFragmentShaderID;
									break;
		} 
		GLint nCompileSuccess = GL_FALSE;
		glGetShaderiv(glLastShader, GL_COMPILE_STATUS, &nCompileSuccess);
		if ((glGetError() != GL_NO_ERROR) || (nCompileSuccess != GL_TRUE))
		{
			int nLogLength = 0;
			int nMaxLength = 0;

			glGetShaderiv(glLastShader, GL_INFO_LOG_LENGTH, &nMaxLength);
			char* cInfo = new char[nMaxLength];
			glGetShaderInfoLog(glLastShader, nMaxLength, &nLogLength, cInfo);

			std::cout << cInfo << std::endl;
			delete[] cInfo;
			return ERROR_CANNOT_COMPILE_SHADER;
		}
		return 0;
	}
	return ERROR_CANNOT_OPEN_SHADER_FILE;
}

