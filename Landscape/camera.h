#pragma once
#include "GL\glew.h"
#include "GL\freeglut.h"
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\transform.hpp>
#include "Shader.h"

enum Direction
{
	Left,
	Forward,
	Backward,
	Right,
	Up,
	Down
};

class Camera {
	private:
		glm::vec3 pos = glm::vec3(39.0f, 57.0f, -35.0f);
		glm::vec3 front = glm::vec3(0.1f, -0.5f, 1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4 modelView;
		glm::mat4 perspective;
		float pitch;
		float yaw;
	public:
		void moveCamera(Direction direction, float amount);
		void rotateCamera(float x, float y);
		Camera();
		Camera(float ratio);
		Camera(const glm::mat4 &gPerspectiveMatrix);
		Camera(const glm::vec3 &pos, const glm::vec3 & front, const glm::vec3 &up, const glm::mat4 &gPerspectiveMatrix);
		~Camera();

		glm::mat4 getModelView()
		{
			return modelView;
		}
		glm::mat4 getPerspective()
		{
			return perspective;
		}

		void registerShader(Shader &shader)
		{

		}

		float getXPos()
		{
			return pos.x;
		}
		float getZPos()
		{
			return pos.z;
		}
		float getYPos()
		{
			return pos.y;
		}
		float getYFront()
		{
			return front.y;
		}
		float getXFront()
		{
			return front.x;
		}
		float getZFront()
		{
			return front.z;
		}
		void look();
};