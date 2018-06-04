#pragma once

#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm\gtc\type_ptr.hpp"

#include "shaders\Shader.h"

#include "ObjectLoader\3dStruct\threeDModel.h"
#include "ObjectLoader\OBJLoader.h"

void createModel(ThreeDModel &model,  Shader *shader, OBJLoader &objLoader, char * address);
void modelTranslation(glm::mat4 & matrix, glm::vec3 translation);
void modelScale(glm::mat4 & matrix, glm::vec3 scale);
void modelRotation(glm::mat4 & matrix, float amount, glm::vec3 axis);

class Spaceship
{
public:

	Spaceship();
	Spaceship(Shader *shader, OBJLoader &objLoader);
	void render();

	// Getters
	ThreeDModel & getModel() { return model; }
	glm::mat4 & getModelViewMatrix() { return modelViewMatrix; }
	glm::vec3 getCoordinates() { return coordinates; }
	float getSpeed() { return speed; }

	// Setters
	void setModelViewMatrix(glm::mat4 matrix) { this->modelViewMatrix = matrix; }
	void setShader(Shader *shader) { modelShader = shader; }
	void setCoordinates(glm::vec3 coord) { coordinates = coord; }
	void setSpeed(float speed) { this->speed = speed; }
	// MODIFIERS

	void translate(glm::vec3 translationMatrix);
	void scale(glm::vec3 translationMatrix);
	void rotate(float amount, glm::vec3 axis);
	void changeCoordinates(glm::vec3 values);

private:
	ThreeDModel model;
	glm::mat4 modelViewMatrix;
	Shader *modelShader;
	glm::vec3 coordinates;
	float speed;
};

class Skybox
{
public:

	Skybox();
	Skybox(Shader *shader, OBJLoader &objLoader);
	void render();

	// Getters
	ThreeDModel & getModel() { return model; }
	glm::mat4 getModelViewMatrix() { return modelViewMatrix; }

	// Setters
	void setModelViewMatrix(glm::mat4 matrix) { this->modelViewMatrix = matrix; }
	void setShader(Shader *shader) { modelShader = shader; }
	void setCoordinates(glm::vec3 coord) { coordinates = coord; }

	// MODIFIERS

	void translate(glm::vec3 translationMatrix);
	void scale(glm::vec3 translationMatrix);
	void changeCoordinates(glm::vec3 values);

private:
	ThreeDModel model;
	glm::mat4 modelViewMatrix;
	Shader *modelShader;
	glm::vec3 coordinates;
};

class EngineFlame
{
public:

	EngineFlame();
	EngineFlame(Shader *shader, OBJLoader &objLoader);
	void render();

	// Getters
	ThreeDModel & getModel() { return model; }
	glm::mat4 getModelViewMatrix() { return modelViewMatrix; }

	// Setters
	void setModelViewMatrix(glm::mat4 matrix) { this->modelViewMatrix = matrix; }
	void setShader(Shader *shader) { modelShader = shader; }

	// MODIFIERS

	void translate(glm::vec3 translationMatrix);
	void scale(glm::vec3 translationMatrix);
	void rotate(float amount, glm::vec3 axis);

	// REQUESTERS
	void sendShipSpeed(float speed);

private:
	ThreeDModel model;
	glm::mat4 modelViewMatrix;
	Shader *modelShader;
	void calculateFlmaeSize(float speed);
	bool flame;
	float flameSize;
	int flameState;
	float flameLoop;
};

class Planet
{
public:

	Planet();
	Planet(Shader *shader, OBJLoader &objLoader, char *address);
	void render();

	// Getters
	ThreeDModel & getModel() { return model; }
	glm::mat4 & getModelViewMatrix() { return modelViewMatrix; }
	float getRotationAngle() { return rotationAngleY; }
	glm::vec3 getCoordinates() { return coordinates; }

	// Setters
	void setModelViewMatrix(glm::mat4 matrix) { this->modelViewMatrix = matrix; }
	void setShader(Shader *shader) { modelShader = shader; }
	void setCoordinates(glm::vec3 coord) { coordinates = coord; }
	void setRotationSpeed(float speed) { rotationSpeed = speed; }
	void setPlanetTilt(float angle) { planetTilt = angle; }
	void setScaleSize(float size) { scaleSize = size; }

	// MODIFIERS
	void translate(glm::vec3 translationMatrix);
	void scale(glm::vec3 translationMatrix);
	void rotating();
	void changeCoordinates(glm::vec3 values);
	void enableRotation();
	void disableRotation();

private:
	ThreeDModel model;
	glm::mat4 modelViewMatrix;
	Shader *modelShader;
	
	glm::vec3 coordinates;

	bool rotation;
	float rotationSpeed;
	float rotationAngleY;
	float planetTilt;
	float radius;
	float scaleSize;

	// MODIFIERS
	void rotate(float amount, glm::vec3 axis);
};