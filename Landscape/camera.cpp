#include "camera.h"

Camera::Camera()
{
	modelView = glm::lookAt(pos, front, up);
	perspective = glm::perspective(45.0f, 1.0f, 1.0f, 10000.0f);
}

Camera::Camera(float ratio)
{
	modelView = glm::lookAt(pos, front, up);
	perspective = glm::perspective(45.0f, ratio, 1.0f, 10000.0f);
}

Camera::Camera(const glm::mat4 &gPerspectiveMatrix)
{
	perspective = gPerspectiveMatrix;
	modelView = glm::lookAt(pos, front, up);
}

Camera::Camera(const glm::vec3 &pos, const glm::vec3 & front, const glm::vec3 &up, const glm::mat4 &gPerspectiveMatrix)
{
	this->pos = pos;
	this->front = front;
	this->up = up;

	modelView = glm::lookAt(pos, front, up);
	perspective = gPerspectiveMatrix;
}

void Camera::moveCamera(Direction direction, float amount)
{
	switch (direction)
	{
		case Left: pos -= amount *  glm::cross(up, front);
			break;
		case Right: pos+= amount *  glm::cross(up, front);
			break;
		case Forward: pos += amount *  front;
			break;
		case Backward: pos -= amount *  front;
			break;
		case Up: pos += amount *  up;
			break;
		case Down: pos -= amount *  up;
			break;
	}
}

void  Camera::rotateCamera(float x, float y)
{
	front.x = cos(glm::radians(x)) * cos(glm::radians(y));
	front.y = sin(glm::radians(y));
	front.z = sin(glm::radians(x)) * cos(glm::radians(y));
}


void Camera::look()
{
	modelView = glm::lookAt(pos, front+pos, up);
	gluLookAt(pos.x, pos.y, pos.z, pos.x + front.x, pos.y + front.y, pos.z + front.z, up.x, up.y, up.z);
}

Camera::~Camera()
{

}