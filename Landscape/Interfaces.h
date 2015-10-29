#pragma once
#include "Shader.h"
#include <vector>

class Drawable
{
public:
	inline GLboolean canBeDrawn()
	{
		return readyToDraw;
	}
	std::string getDrawIdentifier() const
	{
		return drawIdentifier;
	}
	virtual GLboolean draw() = 0;
protected:
	GLboolean readyToDraw = true;
	std::string drawIdentifier = "";
};

class Updateable
{
public:
	inline GLboolean canBeUpdated()
	{
		return readyToUpdate;
	}
	void updateShaders(ShaderUpdate &update)
	{
		std::vector<Shader>::iterator shaderIt = myShader.begin();
		while (shaderIt != myShader.end())
		{
			if (update.updateMask & UPDATEMASK_TIME)
			{
				(*shaderIt).setElapsedTime(update.fTime);
			}
			if (update.updateMask & UPDATEMASK_MODELVIEW)
			{
				(*shaderIt).setModelViewMatrix(update.gModelViewMatrix);
			}
			shaderIt++;
		}
	}
	void updateShadersModelView(glm::mat4 &gModelViewMatrix)
	{
		std::vector<Shader>::iterator shaderIt = myShader.begin();
		while (shaderIt != myShader.end())
		{
			(*shaderIt).setModelViewMatrix(gModelViewMatrix);
			shaderIt++;
		}
	}
	void updateShadersPerspective(glm::mat4 &gPerspectiveMatrix)
	{
		std::vector<Shader>::iterator shaderIt = myShader.begin();
		while (shaderIt != myShader.end())
		{
			(*shaderIt).setPerspectiveMatrix(gPerspectiveMatrix);
			shaderIt++;
		}
	}
	void updateShaderTime(GLfloat fTime)
	{
		std::vector<Shader>::iterator shaderIt = myShader.begin();
		while (shaderIt != myShader.end())
		{
			(*shaderIt).setElapsedTime(fTime);
			shaderIt++;
		}
	}
	std::string getUpdateIdentifier() const
	{
		return updateIdentifier;
	}
protected:
	GLboolean readyToUpdate = true;
	virtual GLboolean update(GLfloat time) = 0;
	std::vector<Shader> myShader;
	std::string updateIdentifier = "";
};

class Initialable
{
public:
	inline GLboolean isInitialized()
	{
		return initalized;
	}
protected:
	GLboolean initalized = false;
	virtual bool init() = 0;
};