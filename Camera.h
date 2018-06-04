#pragma once
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class SurroundingCamera
{
public:
	
	SurroundingCamera();
	void resetCamera();

	// GETTERS
	glm::mat4 getViewMatrix() { return viewMatrix; }

	// SETTERS
	void setCameraZoom(float value);
	void setViewMatrix(glm::mat4 matrix);

	// MODIFIERS
	void translateCamera(glm::vec3 values);
	void rotateCamera(float angleDegrees, glm::vec3 axis);

private:
	glm::vec3 cameraZoom;
	glm::mat4 viewMatrix;

};

class CockpitCamera
{
public:

	CockpitCamera();
	void resetCamera();

	// GETTERS
	glm::mat4 getViewMatrix() { return viewMatrix; }

	// SETTERS
	void setCameraZoom(float value);
	void setViewMatrix(glm::mat4 matrix);

	// MODIFIERS
	void translateCamera(glm::vec3 values);
	void rotateCamera(float angleDegrees, glm::vec3 axis);

private:
	glm::vec3 cameraZoom;
	glm::mat4 viewMatrix;

};



class SpectateCamera
{
public:
	SpectateCamera();

	glm::mat4 spectate(float X, float Y, float Z, float zoom);

private:

	float angle;

};


class LookAtCamera
{

public:
	LookAtCamera();
	
	// GETTERS
	glm::mat4 getViewMatrix() { return viewMatrix; }

	// SETTERS
	void setCameraZoom(float value);
	void setViewMatrix(glm::mat4 matrix);

	// MODIFIERS
	void translateCamera(glm::vec3 values);
	void rotateCamera(float angleDegrees, glm::vec3 axis);

private:
	glm::vec3 cameraZoom;
	glm::mat4 viewMatrix;
};