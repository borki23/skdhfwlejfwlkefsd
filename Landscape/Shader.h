#pragma once
#include "GL\glew.h"
#include <string>
#include <map>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\transform.hpp>

#define ERROR_CANNOT_OPEN_SHADER_FILE 1
#define ERROR_CANNOT_COMPILE_SHADER 2
#define ERROR_SHADER_NOT_COMPILED 3
#define ERROR_CANNOT_LINK_PROGRAM 4
#define ERROR_CANNOT_FIND_UNIFORM 5

#define UPDATEMASK_TIME 1
#define UPDATEMASK_MODELVIEW 2

enum class ValueType
{
	COLOR3F
};

struct Color3f
{
	float r;
	float g;
	float b;
};

struct ShaderUpdate
{
	GLfloat fTime;
	glm::mat4 gModelViewMatrix;
	unsigned int updateMask;
	//glm::mat4 gModelViewMatrix;
};

class Shader
{
	private:
		const GLchar* MODELVIEWNAME = "gl_ModelView";
		const GLchar* PROJECTIONNAME = "gl_Projection";
		const GLchar* TIMENAME = "gl_Time";

		GLuint m_nProgramID;
		std::string m_sCurrentFragmentShader;
		GLuint m_nFragmentShaderID;
		std::string m_sCurrentVertexShader;
		GLuint m_nVertexShaderID;
		GLint m_nModelViewMatrixPos;
		glm::mat4 m_currentModelViewMatrix;
		GLint m_nPerspectiveMatrixPos;
		glm::mat4 m_currentPerspectiveMatrix;
		GLint m_nCurrentTimePos;
		GLfloat m_nCurrentTime;
	public:
		Shader();
		Shader(glm::mat4 &gModelViewMatrix, glm::mat4 &gPerspectiveMatrix);
		~Shader();

		GLuint SetShader(const std::string & sShader, GLenum eType);
		GLuint initProgram();
	
		void bind()
		{
			glUseProgram(m_nProgramID);
		}
		void unbind()
		{
			glUseProgram(NULL);
		}
		void setElapsedTime(GLfloat fTime)
		{
			m_nCurrentTime = fTime;
			glUniform1f(m_nCurrentTimePos, m_nCurrentTime);
		}

		void setModelViewMatrix(glm::mat4 &gModelViewMatrix)
		{
			m_currentModelViewMatrix = gModelViewMatrix;
			glUniformMatrix4fv(m_nModelViewMatrixPos, 1, GL_FALSE, glm::value_ptr(m_currentModelViewMatrix));
		}

		void setPerspectiveMatrix(glm::mat4 &gPerspectiveMatrix)
		{
			m_currentPerspectiveMatrix = gPerspectiveMatrix;
			glUniformMatrix4fv(m_nPerspectiveMatrixPos, 1, GL_FALSE, glm::value_ptr(m_currentPerspectiveMatrix));
		}

		/*static void setStaticModelView(const glm::mat4 &gModelViewMatrix)
		{
			staticModelView = gModelViewMatrix;
		}

		static void setStaticPerspective(const glm::mat4 &gPerspectiveMatrix)
		{
			staticPerspectiveView = gPerspectiveMatrix;
		}

		void updateModelView()
		{
			glUniformMatrix4fv(m_nModelViewMatrixPos, 1, GL_FALSE, glm::value_ptr(m_currentModelViewMatrix));
		}

		void updatePerspective()
		{
			glUniformMatrix4fv(m_nPerspectiveMatrixPos, 1, GL_FALSE, glm::value_ptr(staticPerspectiveView));
		}*/
};