#include "Camera.h"

SurroundingCamera::SurroundingCamera()
{
	viewMatrix = glm::mat4(1.0);
}

void SurroundingCamera::resetCamera()
{
	viewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, -100,  -100));
}

void SurroundingCamera::setCameraZoom(float value)
{
	cameraZoom = glm::vec3(0, 0, value);
	viewMatrix = glm::translate(viewMatrix,cameraZoom);
}
void SurroundingCamera::setViewMatrix(glm::mat4 matrix)
{
	this->viewMatrix = matrix;
}

void SurroundingCamera::translateCamera(glm::vec3 values)
{
	this->viewMatrix = glm::translate(viewMatrix, values);
}
void SurroundingCamera::rotateCamera(float angleDegrees, glm::vec3 axis)
{
	this->viewMatrix = glm::rotate(viewMatrix, angleDegrees, axis);
}


CockpitCamera::CockpitCamera()
{
	viewMatrix = glm::mat4(1.0);
}

void CockpitCamera::resetCamera()
{
	viewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, -20, 50));
}

void CockpitCamera::setCameraZoom(float value)
{
	cameraZoom = glm::vec3(0, 0, value);
	viewMatrix = glm::translate(viewMatrix, cameraZoom);
}
void CockpitCamera::setViewMatrix(glm::mat4 matrix)
{
	this->viewMatrix = matrix;
}

void CockpitCamera::translateCamera(glm::vec3 values)
{
	this->viewMatrix = glm::translate(viewMatrix, values);
}
void CockpitCamera::rotateCamera(float angleDegrees, glm::vec3 axis)
{
	this->viewMatrix = glm::rotate(viewMatrix, angleDegrees, axis);
}


SpectateCamera::SpectateCamera()
{
	this->angle = 0;
}
glm::mat4 SpectateCamera::spectate(float X, float Y, float Z, float zoom)
{
	if (angle > 0)
	{
		if (angle > 0.5)
		{
			angle = angle - 0.002f;
		}
		else
		{
			angle = -3;
		}
	}
	else
	{
		if (angle < -0.5)
		{
			angle = angle + 0.002f;
		}
		else
		{
			angle = 3;
		}
	}

	glm::mat4 viewingMatrix;

	viewingMatrix = glm::lookAt(glm::vec3(X + 400 * sin(angle), Y + 200, Z + 400 * cos(angle)), glm::vec3(X, Y, Z), glm::vec3(0, 1, 0));

	viewingMatrix = glm::translate(viewingMatrix, glm::vec3(sin(angle) * zoom, 0, cos(angle) * zoom));

	return viewingMatrix;
}