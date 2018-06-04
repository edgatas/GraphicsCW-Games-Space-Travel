#include "models.h"

#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "gl/glew.h"
#include "gl/wglew.h"

void createModel(ThreeDModel &model, Shader *shader, OBJLoader &objLoader, char * address)
{
	if (objLoader.loadModel(address, model))
	{
		cout << " model loaded " << endl;

		model.calcCentrePoint(); // calculate center of model
		model.centreOnZero(); // translate to center

		model.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.

											 //turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		model.initDrawElements();
		model.initVBO(shader);
		model.deleteVertexFaceData();
	}
	else
	{
		cout << " model failed to load " << endl;
	}
}

void modelTranslation(glm::mat4 & matrix, glm::vec3 translation)
{
	matrix = glm::translate(matrix, translation);
}
void modelScale(glm::mat4 & matrix, glm::vec3 scale)
{
	matrix = glm::scale(matrix, scale);
}
void modelRotation(glm::mat4 & matrix, float amount, glm::vec3 axis)
{
	matrix = glm::rotate(matrix, amount, axis);
}

// SPACESHIP
Spaceship::Spaceship()
{

}

Spaceship::Spaceship(Shader *shader, OBJLoader &objLoader)
{
	char * modelAddress = "Models/WraithSpaceShip/Wraith Raider Starship.obj";
	createModel(model,shader, objLoader, modelAddress);
	this->modelShader = shader;
}

void Spaceship::render()
{




	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(modelShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(modelShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
	model.drawElementsUsingVBO(modelShader);
}

void Spaceship::translate(glm::vec3 translationMatrix)
{
	modelTranslation(modelViewMatrix, translationMatrix);
}
void Spaceship::scale(glm::vec3 scaleMatrix)
{
	modelScale(modelViewMatrix, scaleMatrix);
}
void Spaceship::rotate(float amount, glm::vec3 axis)
{
	modelRotation(modelViewMatrix, amount, axis);
}
void Spaceship::changeCoordinates(glm::vec3 values)
{
	coordinates = coordinates + values;
}
// SPACESHIP END

// SKYBOX
Skybox::Skybox()
{

}

Skybox::Skybox(Shader *shader, OBJLoader &objLoader)
{
	char * modelAddress = "Models/Skybox/Skybox.obj";
	createModel(model, shader, objLoader, modelAddress);
	this->modelShader = shader;
	this->coordinates = glm::vec3(0, 0, 0);
}

void Skybox::render()
{
	translate(coordinates);
	scale(glm::vec3(10000, 10000, 10000));

	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);

	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(modelShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(modelShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
	model.drawElementsUsingVBO(modelShader);

	glDepthMask(true);
	glEnable(GL_DEPTH_TEST);
}

void Skybox::translate(glm::vec3 translationMatrix)
{
	modelTranslation(modelViewMatrix, translationMatrix);

}
void Skybox::scale(glm::vec3 scaleMatrix)
{
	modelScale(modelViewMatrix, scaleMatrix);
}
void Skybox::changeCoordinates(glm::vec3 values)
{
	coordinates = coordinates + values;
}
// SKYBOX END

// ENGINE TRAIL
EngineFlame::EngineFlame()
{

}

EngineFlame::EngineFlame(Shader *shader, OBJLoader &objLoader)
{
	char * modelAddress = "Models/WraithSpaceShip/EngineTrail.obj";
	createModel(model, shader, objLoader, modelAddress);
	this->modelShader = shader;
	this->flame = true;
	this->flameSize = 1;
	this->flameState = 0;
	this->flameLoop = 0;
}

void EngineFlame::render()
{
	translate(glm::vec3(0, 0, 250));
	scale(glm::vec3(1, 1, flameSize));

	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(modelShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(modelShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
	model.drawElementsUsingVBO(modelShader);
}

void EngineFlame::translate(glm::vec3 translationMatrix)
{
	modelTranslation(modelViewMatrix, translationMatrix);

}
void EngineFlame::scale(glm::vec3 scaleMatrix)
{
	modelScale(modelViewMatrix, scaleMatrix);
}
void EngineFlame::rotate(float amount, glm::vec3 axis)
{
	modelRotation(modelViewMatrix, amount, axis);
}
void EngineFlame::sendShipSpeed(float speed)
{
	calculateFlmaeSize(speed);
}
void EngineFlame::calculateFlmaeSize(float speed)
{
	if (speed <= 0)
	{

		if (flameState == 0)
		{
			flameSize = 1 * -(speed * 20) + (flameLoop / 10 * -speed );
			flameLoop += 20;
		}
		else
		{
			flameSize = 1 * -(speed * 20) + (flameLoop / 10 * -speed);
			flameLoop -= 20;
		}
		if (flameLoop > 100) { flameState = 1; }
		if (flameLoop < 10) { flameState = 0; }
	}
}

// ENGINE TRAIL END

// PLANET START
Planet::Planet()
{

}

Planet::Planet(Shader *shader, OBJLoader &objLoader, char *address)
{
	char * modelAddress = address;
	createModel(model, shader, objLoader, modelAddress);
	this->modelShader = shader;
	this->rotationAngleY = 0;
	this->planetTilt = 0;
}

void Planet::render()
{
	rotating();

	
	translate(coordinates);
	rotate(planetTilt, glm::vec3(0, 0, 1));
	rotate(getRotationAngle(), glm::vec3(0, 1, 0));
	scale(glm::vec3(scaleSize, scaleSize, scaleSize));

	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(modelShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(modelShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
	model.drawElementsUsingVBO(modelShader);
}

void Planet::translate(glm::vec3 translationMatrix)
{
	modelTranslation(modelViewMatrix, translationMatrix);
}
void Planet::scale(glm::vec3 scaleMatrix)
{
	modelScale(modelViewMatrix, scaleMatrix);
}
void Planet::rotate(float amount, glm::vec3 axis)
{
	modelRotation(modelViewMatrix, amount, axis);
}
void Planet::rotating()
{
	if (rotation)
	{
		if (rotationAngleY == 360) { rotationAngleY = 0; }
		rotationAngleY += rotationSpeed;
	}

}
void Planet::changeCoordinates(glm::vec3 values)
{
	coordinates = coordinates + values;
}

void Planet::enableRotation()
{
	this->rotation = true;
}
void Planet::disableRotation()
{
	this->rotation = false;
}
// PLANET END