//OPENGL 3.2 EXAMPLE

//NB: DEPRECATED COMMANDS WILL NOT FUNCTION.
//SEE OPENGL QUICK REFERENCE FOR DETAILS

#include <windows.h>		// Header File For Windows
//#include <gl\gl.h>			// Header File For The OpenGL32 Library  
//#include <gl\glu.h>			// Header File For The GLu32 Library 
//-- OPENGL FILES INCLUDED IN GLEW see below

#include <math.h>     //sin and cos included in this header file.
#include <vector>

#include <time.h>

#include <cmath>

#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
                              //Shader.h has been created using common glsl structure

#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm\gtc\type_ptr.hpp"

//--- OpenGL ---
#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")
//--------------

//--- Objects Loader ---
#include "ObjectLoader\OBJLoader.h"
OBJLoader objLoader;
//--------------

//--- Objects ---
#include "models.h"
glm::mat4 cRotation;

Spaceship *spaceship;
Skybox *skybox;
EngineFlame *engineFlame;
Planet *earth;
Planet *sun;
Planet *smallEarth;
Planet *winter;


//Material properties
float Material_Ambient[4] = { 0.05f, 0.0001f, 0.0001f, 1.0f };
float Ship_Material_Ambient[4] = { 0.05f, 0.0001f, 0.0001f, 1.0f };
float Material_Diffuse[4] = { 0.8f, 0.8f, 0.5f, 1.0f };
float Material_Specular[4] = { 0.9f,0.9f,0.8f,1.0f };
float Material_Shininess = 20;

//Light Properties
float Light_Ambient_And_Diffuse[4] = { 0.8f, 0.8f, 0.6f, 1.0f };
float Light_Specular[4] = { 1.0f,1.0f,1.0f,1.0f };
float LightPos[4] = { 0.0f, 0.0f, 1.0f, 0.0f };

// Ship Light positions
float Ship_LightPos[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

//Planet Light Properties
float Planet_Light_Ambient_And_Diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float Planet_Light_Specular[4] = { 1.0f,1.0f,1.0f,1.0f };
float Planet_LightPos[4] = { 0.0f, 0.0f, 1.0f, 0.0f };

//--- Camera ---
#include "Camera.h"
SurroundingCamera *surCamera;
SpectateCamera *specCamera;
CockpitCamera *cpitCamera;
int cameraState = 0;
int cameraIdle = 0;
float cameraYaw = 0;
float cameraPitch = 0;
float cameraRoll = 0;
float cameraZoom = -100;

bool keys[256];			// Array Used in the keyboard method.

int	mouse_x=0, mouse_y=0;
int mouseXDiff[] = { 0,0,0 };
int mouseYDiff[] = { 0,0,0 };
int mouseDiffIndex = 0;

int zDelta = 0;
float viewZoom = 1;
int mouseXLast = 0, mouseYLast = 0; // For camera movement
bool leftPressed = false;
bool rightPressed = false;
bool wheelMoved = false;
bool positionUpdate = false;

float FOV = 60;

// --- Object Moevemnt ontrol --- //
float mX = 192567;
float mY = 112673;
float mZ = 37751.5;
float oldmX = 0;
float oldmY = 0;
float oldmZ = 0;

int detectState = 0;
float diffX = 0;
float diffY = 0;
float diffZ = 0;

float rotation = 0;

float objectSpeed = 0;
float objectXspeed = 0;
float objectYspeed = 0;
float objectZspeed = 0;

float objectMovementYaw = 0;;
float objectMovementRoll = 0;
float objectMovementPitch = 0;

bool angleJump = false;

float speedF = 0;
float speedS = 0;

Shader *objectShader;  ///shader object 
Shader *objectShaderCopy;
Shader *spaceShipShader;
Shader *skyBoxShader;
Shader *planetShader;
Shader *sunShader;

bool earthDetection(float speedX, float speedY, float speedZ);
bool winterDetection(float speedX, float speedY, float speedZ);

float amount = 0;
float temp = 0.002f;

#include <iostream>
#include <iomanip>
using namespace std;

int screenWidth=800, screenHeight=480;

void updateTransform(float xinc, float yinc, float zinc);

//OPENGL FUNCTION PROTOTYPES
void init();				//called in winmain when the program starts.
void reshape(int width, int height);
void display();				//called in winmain to draw everything to the screen
void sunDetection();
void keyboard();     //called to check key presses

/*************    START OF OPENGL FUNCTIONS   ****************/

glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing
glm::mat4 objectRotation;
glm::quat q;

// TESTING
int keyState = 0;
float angle = 0;

float testing;

float testing1 = 0;
float testing2 = 0;
float testing3 = 0;


float pos[3] = {0.0f, 0.0f, 110000};


void init()
{

	surCamera = new SurroundingCamera();
	specCamera = new SpectateCamera();
	cpitCamera = new CockpitCamera();
	testing = 1.0f;

	glClearColor(1.0,0.0,0.0,0.0);						//sets the clear colour to red
	//glClear(GL_COLOR_BUFFER_BIT) in the display function//will clear the buffer to this colour.

	// Shader
	objectShader = new Shader();
	if (!objectShader->load("BasicView", "glslfiles/basicTransformations.vert", "glslfiles/basicTransformations.frag"))
	{
		cout << "failed to load object shader" << endl;
	}
	spaceShipShader = new Shader();
	if (!spaceShipShader->load("CopyOfBasicView", "glslfiles/spaceShipShader.vert", "glslfiles/spaceShipShader.frag"))
	{
		cout << "failed to load spaceShip shader" << endl;
	}
	skyBoxShader = new Shader();
	if (!skyBoxShader->load("skyBoxShader", "glslfiles/skyBoxShader.vert", "glslfiles/skyBoxShader.frag"))
	{
		cout << "failed to load Skybox Shader" << endl;
	}
	planetShader = new Shader();
	if (!planetShader->load("planetShader", "glslfiles/Planet.vert", "glslfiles/Planet.frag"))
	{
		cout << "failed to load Planet Shader" << endl;
	}
	sunShader = new Shader();
	if (!sunShader->load("sunShader", "glslfiles/Sun.vert", "glslfiles/Sun.frag"))
	{
		cout << "failed to load Sun Shader" << endl;
	}

	// SPACESHIP LOADER
	spaceship = new Spaceship(spaceShipShader, objLoader);
	// SPACESHIP END 

	// SKYBOX LOADER
	skybox = new Skybox(skyBoxShader, objLoader);
	// SKYBOX END

	// ENGINE TRAIL
	engineFlame = new EngineFlame(sunShader, objLoader);
	// ENGINE TRAIL END

	// PLANET EARTH
	earth = new Planet(planetShader, objLoader, "Models/Planets/Earth/Earth.obj");
	sun = new Planet(sunShader, objLoader, "Models/Planets/Sun/Sun.obj");
	winter = new Planet(planetShader, objLoader, "Models/Planets/Winter/Winter.obj");

	// Setting object settings

	sun->setCoordinates(glm::vec3(0, 0, 10000000));
	sun->enableRotation();
	sun->setRotationSpeed(0.1f);
	sun->setScaleSize(100);

	earth->setCoordinates(glm::vec3(200000, 0, 0));
	earth->disableRotation();
	earth->setScaleSize(1.0f);
	earth->setPlanetTilt(23.5f);

	winter->setCoordinates(glm::vec3(-2000000, 500000, 1000000));
	winter->disableRotation();
	winter->setScaleSize(10.0f);
	winter->setPlanetTilt(0.0f);

	

glEnable(GL_DEPTH_TEST);
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width;
	screenHeight = height;

	glViewport(0, 0, width, height);						// set Viewport dimensions

	ProjectionMatrix = glm::perspective(FOV, (GLfloat)screenWidth / (GLfloat)screenHeight, 1.0f, 20000000.0f);
	cout << FOV << endl;
}



void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	amount += temp;
	if (amount > 1.0f || amount < -0.5f)
		temp = -temp;
	//amount = 0;


	GLuint matLocation;

	// DIfferent Camera views

	glm::mat4 viewingMatrix;
	switch (cameraState)
	{
	case 0:
		surCamera->resetCamera();
		surCamera->setCameraZoom(viewZoom);
		surCamera->rotateCamera(cameraRoll, glm::vec3(0, 0, 1));
		surCamera->rotateCamera(cameraPitch, glm::vec3(1, 0, 0));
		surCamera->rotateCamera(cameraYaw, glm::vec3(0, 1, 0));
		surCamera->translateCamera(glm::vec3(-mX, -mY, -mZ));

		viewingMatrix = surCamera->getViewMatrix();
		break;

	case 1:
		cpitCamera->resetCamera();
		cpitCamera->setCameraZoom(0);
		cpitCamera->rotateCamera(cameraRoll, glm::vec3(0, 0, 1));
		cpitCamera->rotateCamera(cameraPitch, glm::vec3(1, 0, 0));
		cpitCamera->rotateCamera(cameraYaw, glm::vec3(0, 1, 0));
		cpitCamera->translateCamera(glm::vec3(-mX, -mY, -mZ));

		viewingMatrix = cpitCamera->getViewMatrix();
		break;

	case 2:
		viewingMatrix = specCamera->spectate(mX, mY, mZ, -25);
		break;
	}

	glUseProgram(skyBoxShader->handle());
	glUniform1f(glGetUniformLocation(skyBoxShader->handle(), "displacement"), amount);
	matLocation = glGetUniformLocation(skyBoxShader->handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skyBoxShader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	// LIGHT first shader
	glUniform4fv(glGetUniformLocation(skyBoxShader->handle(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(skyBoxShader->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(skyBoxShader->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(skyBoxShader->handle(), "light_specular"), 1, Light_Specular);

	glUniform4fv(glGetUniformLocation(skyBoxShader->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(skyBoxShader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(skyBoxShader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(skyBoxShader->handle(), "material_shininess"), Material_Shininess);
	// LIGHT END

	// SKYBOX. Last to render without depth testing and mask.
	glUseProgram(skyBoxShader->handle());
	skybox->setModelViewMatrix(viewingMatrix);
	skybox->setCoordinates(glm::vec3(mX, mY, mZ));
	skybox->render();
	// SKYBOX END



	glUseProgram(planetShader->handle());
	glUniform1f(glGetUniformLocation(planetShader->handle(), "displacement"), amount);
	matLocation = glGetUniformLocation(planetShader->handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(planetShader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	// PLANET SHADER LIGHTS
	glUniform4fv(glGetUniformLocation(planetShader->handle(), "LightPos"), 1, Ship_LightPos);
	glUniform4fv(glGetUniformLocation(planetShader->handle(), "light_ambient"), 1, Planet_Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(planetShader->handle(), "light_diffuse"), 1, Planet_Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(planetShader->handle(), "light_specular"), 1, Planet_Light_Specular);

	glUniform4fv(glGetUniformLocation(planetShader->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(planetShader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(planetShader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(planetShader->handle(), "material_shininess"), Material_Shininess);
	// PLANET SHADER LIGHTS END

	// PLANET EARTH
	glUseProgram(planetShader->handle());
	earth->setModelViewMatrix(viewingMatrix);
	earth->render();

	winter->setModelViewMatrix(viewingMatrix);
	winter->render();
	// PLANET EARTH END

	// PLANET SHADER LIGHT PREPARATION
	glUseProgram(sunShader->handle());
	glUniform1f(glGetUniformLocation(sunShader->handle(), "displacement"), amount);
	matLocation = glGetUniformLocation(sunShader->handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(sunShader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	// PLANET SHADER LIGHTS
	glUniform4fv(glGetUniformLocation(sunShader->handle(), "LightPos"), 1, Planet_LightPos);
	glUniform4fv(glGetUniformLocation(sunShader->handle(), "light_ambient"), 1, Planet_Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(sunShader->handle(), "light_diffuse"), 1, Planet_Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(sunShader->handle(), "light_specular"), 1, Planet_Light_Specular);

	glUniform4fv(glGetUniformLocation(sunShader->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(sunShader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(sunShader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(sunShader->handle(), "material_shininess"), Material_Shininess);
	// PLANET SHADER LIGHTS END

	// PLANET SUN
	glUseProgram(sunShader->handle());
	sun->setModelViewMatrix(viewingMatrix);
	sun->render();
	// PLANET SUN END


	// CUBE WITH ROTATIONS
	// Static cube that spawns at the same place as the main cube. However, this one is just a static object
	//glUseProgram(objectShader->handle());
	ModelViewMatrix = viewingMatrix * objectRotation;
	//testCube2->setModelViewMatrix(ModelViewMatrix);
	//testCube2->render();
	// CUBE WITH ROTATIONS END 

	// --- This needs to be moved to somewhere else. Possible object movement class or something
	if (angleJump)
	{
		if (cameraYaw > 180)
		{
			//cout << "State1" << endl;
			objectMovementYaw = objectMovementYaw + 360.f;
		}
		else
		{
			//cout << "State2" << endl;
			objectMovementYaw = objectMovementYaw - 360.f;
		}
		angleJump = false;
	}
	// Spaceship rotation speed

	if (cameraState == 1)
	{
		objectMovementYaw = objectMovementYaw + float(cameraYaw - objectMovementYaw);
		objectMovementRoll = objectMovementRoll + float(cameraRoll - objectMovementRoll);
		objectMovementPitch = objectMovementPitch + float(cameraPitch - objectMovementPitch);
	}
	else
	{
		objectMovementYaw = objectMovementYaw + float(cameraYaw - objectMovementYaw) / 50;
		objectMovementRoll = objectMovementRoll + float(cameraRoll - objectMovementRoll) / 50;
		objectMovementPitch = objectMovementPitch + float(cameraPitch - objectMovementPitch) / 50;
	}


	//cout << "Yaw1: " << cameraYaw << " Roll1: " << objectMovementRoll << " Pitch1: " << cameraPitch;
	//cout << " Yaw2: " << objectMovementYaw << " Roll2: " << objectMovementRoll << " Pitch2: " << objectMovementPitch << endl;


	// Calculations for light position
	glm::vec3 sunPos = sun->getCoordinates();
	glm::vec3 shipPos = spaceship->getCoordinates();
	glm::vec3 diff = sunPos - shipPos;
	float d1 = abs(diff[0]);
	float d2 = abs(diff[1]);
	float d3 = abs(diff[2]);
	float sum = d1 + d2 + d3;

	float dd1 = d1 / sum;
	float dd2 = d2 / sum;
	float dd3 = d3 / sum;

	if (diff[0] > 0)
	{
		Ship_LightPos[0] = dd1;
	}
	else
	{
		Ship_LightPos[0] = -dd1;
	}

	if (diff[1] > 0)
	{
		Ship_LightPos[1] = dd2;
	}
	else
	{
		Ship_LightPos[1] = -dd2;
	}

	if (diff[2] > 0)
	{
		Ship_LightPos[2] = dd3;
	}
	else
	{
		Ship_LightPos[2] = -dd3;
	}


	glUseProgram(spaceShipShader->handle());
	glUniform1f(glGetUniformLocation(spaceShipShader->handle(), "displacement"), amount);
	matLocation = glGetUniformLocation(spaceShipShader->handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(spaceShipShader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	// LIGHT first shader
	glUniform4fv(glGetUniformLocation(spaceShipShader->handle(), "LightPos"), 1, Ship_LightPos);
	glUniform4fv(glGetUniformLocation(spaceShipShader->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(spaceShipShader->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(spaceShipShader->handle(), "light_specular"), 1, Light_Specular);

	glUniform4fv(glGetUniformLocation(spaceShipShader->handle(), "material_ambient"), 1, Ship_Material_Ambient);
	glUniform4fv(glGetUniformLocation(spaceShipShader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(spaceShipShader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(spaceShipShader->handle(), "material_shininess"), Material_Shininess);
	// LIGHT END

	// SPACESHIP

	if (Ship_Material_Ambient[0] < 10)
	{
		glUseProgram(spaceShipShader->handle());
		spaceship->setModelViewMatrix(viewingMatrix);
		spaceship->setCoordinates(glm::vec3(mX, mY, mZ));
		spaceship->translate(spaceship->getCoordinates());
		spaceship->rotate(objectMovementYaw, glm::vec3(0, -1, 0));
		spaceship->rotate(objectMovementRoll, glm::vec3(0, 0, 1));
		spaceship->rotate(objectMovementPitch, glm::vec3(-1, 0, 0));
		spaceship->render();
	}

	cout << spaceship->getCoordinates()[0] << " " << spaceship->getCoordinates()[1] << " " << spaceship->getCoordinates()[2] << endl;

	// SPACESHIP END

	// SPACESHIP ENGINE TRAIL
	if (spaceship->getSpeed()*-100 > 1)
	{
		glUseProgram(sunShader->handle());
	}
	else
	{
		glUseProgram(objectShader->handle());
	}
	engineFlame->setModelViewMatrix(spaceship->getModelViewMatrix());
	engineFlame->sendShipSpeed(spaceship->getSpeed());
	if (Ship_Material_Ambient[0] < 10)
	{
		engineFlame->render();
	}
	
	// SPACESHIP ENGINE TRAIL END

	// End of rendering


	glBindVertexArray(0); //unbind the vertex array object
	
	glUseProgram(0); //turn off the current shader
}


void keyboard()
{
	float spinXinc = 0, spinYinc = 0, spinZinc = 0;
	if (keys[VK_UP])
	{
		spinXinc = 0.01f;
	}
	if (keys[VK_DOWN])
	{
		spinXinc = -0.01f;
	}
	if (keys[VK_LEFT])
	{
		spinYinc = 0.01f;
	}
	if (keys[VK_RIGHT])
	{
		spinYinc = -0.01f;
	}
	if (keys[VK_SPACE])
	{
		spinZinc = 0.01f;
	}
	if (keys[VK_SHIFT])
	{
		spinZinc = -0.01f;
	}
	if (keys['L'])
	{
		FOV -= 0.1f;
		reshape(screenWidth, screenHeight);
	}
	if (keys['K'])
	{
		FOV += 0.1f;
		reshape(screenWidth, screenHeight);
	}

	if (leftPressed)
	{
		int mouseDifX = mouse_x - mouseXLast;
		int mouseDifY = mouse_y - mouseYLast;

		spinXinc += mouseDifY;
		spinYinc += mouseDifX;
	}

	if (rightPressed)
	{
		int tempX = mouse_x - mouseXLast; // difference between X mouse positions
		int tempY = mouse_y - mouseYLast; // difference between Y mouse positions

		// Stonring information about previous values of mouse movement difference
		mouseXDiff[0] = mouseXDiff[1];
		mouseXDiff[1] = mouseXDiff[2];
		mouseXDiff[2] = tempX;

		mouseYDiff[0] = mouseYDiff[1];
		mouseYDiff[1] = mouseYDiff[2];
		mouseYDiff[2] = tempY;

		// Getting average value from movement history. This should smooth movement a bit 
		float mouseDifX = (float)(mouseXDiff[0] + mouseXDiff[1] + mouseXDiff[2]) / 3.0f;
		float mouseDifY = (float)(mouseYDiff[0] + mouseYDiff[1] + mouseYDiff[2]) / 3.0f;

		//cout << " MouseXDiff: " << tempX << " " << mouseXDiff[0] << mouseXDiff[1] << mouseXDiff[2] << " " << mouseDifX << endl;

		// ------ X part of the movement --- //
		cameraYaw += mouseDifX;	// Move X by x diff

		// ------- Y part of the movement --- //
		if (cameraPitch + (mouseDifY / -20) <= 90.0 && cameraPitch + (mouseDifY / -20) >= -90.0)
		{
			cameraPitch += mouseDifY / -2;
		}
		else
		{
			if (cameraPitch > 90) { cameraPitch = 90; }
			if (cameraPitch < -90) { cameraPitch = -90; }
		}
	}


	// Leaving viewing angles only from -360 to 360 for movement purpose
	if (abs(cameraRoll) >= 360)
	{
		cameraRoll = fmod(cameraRoll, 360.0f);
	}

	if (abs(cameraYaw) >= 360)
	{
		cameraYaw = fmod(cameraYaw, 360.0f);
		angleJump = true;
	}
	if (cameraYaw < 0)
	{
		cameraYaw += 360;
		angleJump = true;
	}

	// Calculating ratio at which the object has to move in Y axis.
	float mRatioY = cameraPitch / 90.0f;
	

	// Creating an X coordinates ratio for 3rd person camera ship movement (Moving to the sides)
	float mRatioX = 0;
	float mRatioZ = 0;
	int switchValue = (int)(cameraYaw / 90); // Checking at which quadrant camera is for switch options
	switch (switchValue)
	{
	case 0: // Ratio from 0 to 90 degress  X: 0 -> -1 Z: 1 -> 0
		mRatioX = - (cameraYaw / 90.0f);
		mRatioZ = 1 + mRatioX;
		break;
	case 1: // Ratio from 90 to 180 degress X: -1 -> 0 Z: 0 -> -1
		mRatioX = - ( 1 - (cameraYaw - 90) / 90.0f); 
		mRatioZ = - (1 + mRatioX);
		break;
	case 2: // Ratio from 180 to 270 degress X: 0 -> 1 Z: -1 -> 0
		mRatioX = (cameraYaw - 180) / 90.0f;
		mRatioZ = - (1 - mRatioX);
		break;
	case 3: // Ratio from 270 to 360 degress X: 1 -> 0 Z: 0 -> 1
		mRatioX = (1 - (cameraYaw - 270) / 90.0f);
		mRatioZ = 1 - mRatioX;
		break;
	}

	if (wheelMoved)
	{
		viewZoom += (zDelta / 10);
		if (zDelta > 0)
		{
			if (viewZoom > 0)
			{
				viewZoom += viewZoom * 0.1f;
			}
			else
			{
				viewZoom += viewZoom * -0.1f;
			}
			cout << "First" << endl;
		}
		else
		{
			if (viewZoom > 0)
			{
				viewZoom += viewZoom * -0.1f;
			}
			else
			{
				viewZoom += viewZoom * 0.1f;
			}
			cout << "Second" << endl;
		}
		cout << viewZoom << " " << zDelta << endl;

		if (viewZoom > 100)
		{
			viewZoom = 100;
		}
		wheelMoved = false;
	}

	if (keys[VK_NUMPAD4])
	{
		cameraYaw += 0.1f;
	}
	if (keys[VK_NUMPAD6])
	{
		cameraYaw += -0.1f;
	}
	if (keys[VK_NUMPAD8])
	{
		cameraPitch += 0.1f;
	}
	if (keys[VK_NUMPAD2])
	{
		cameraPitch += -0.1f;
	}
	if (keys[VK_NUMPAD7])
	{
		cameraRoll += -0.1f;
	}
	if (keys[VK_NUMPAD9])
	{
		cameraRoll += 0.1f;
	}
	if (keys[VK_NUMPAD5])
	{
		cameraYaw = 0;
		cameraPitch = 0;
		cameraRoll = 0;
	}


	if (keys['W'])
	{
		// Movement
		objectSpeed -= 0.001f;
		objectZspeed += objectSpeed * mRatioZ;
		objectYspeed += objectSpeed * mRatioY;
		objectXspeed += objectSpeed * mRatioX;
	}

	if (keys['S'])
	{
		// Movement
		objectSpeed += 0.0001f;
		objectZspeed += objectSpeed * mRatioZ;
		objectYspeed += objectSpeed * mRatioY;
		objectXspeed += objectSpeed * mRatioX;
	}

	if (keys['A'])
	{
		speedS -= 0.0001f;
	}
	if (keys['D'])
	{
		speedS += 0.001f;
	}
	if (keys['R'])
	{
		objectSpeed = 0;
		objectXspeed = 0;
		objectYspeed = 0;
		objectZspeed = 0;
	}
	if (keys['F'])
	{
		mX = 0;
		mY = 0;
		mZ = 0;
	}

	if (keys['E'])
	{
		rotation -= 0.1f;
	}
	if (keys['Q'])
	{
		rotation += 0.1f;
	}

	if (keys['C'])
	{
		cameraIdle++;
		if (cameraIdle > 20) // Probably good time to introducse time lapse?
		{
			cameraState++;
			if (cameraState > 2) { cameraState = 0; }
			cameraIdle = 0;
		}
	}

	if (keys['1'])
	{
		keyState = 1;
		cout << "State: " << keyState << endl;
	}
	if (keys['2'])
	{
		keyState = 2;
		cout << "State: " << keyState << endl;
	}
	if (keys['3'])
	{
		keyState = 3;
		cout << "State: " << keyState << endl;
	}
	if (keys['4'])
	{
		keyState = 4;
		cout << "State: " << keyState << endl;
	}
	if (keys['5'])
	{
		keyState = 5;
		cout << "State: " << keyState << endl;
	}
	if (keys['6'])
	{
		keyState = 6;
		cout << "State: " << keyState << endl;
	}

	switch (keyState)
	{
	case 1:
		if (keys['9'])
		{
			testing1 -= 0.1f;
			cout << testing1 << ":" << testing2 << ":" << testing3 << endl;
		}
		if (keys['0'])
		{
			testing1 += 0.1f;
			cout << testing1 << ":" << testing2 << ":" << testing3 << endl;
		}
		break;
	case 2:
		if (keys['9'])
		{
			testing2 -= 0.1f;
			cout << testing1 << ":" << testing2 << ":" << testing3 << endl;
		}
		if (keys['0'])
		{
			testing2 += 0.1f;
			cout << testing1 << ":" << testing2 << ":" << testing3 << endl;
		}
		break;
	case 3:
		if (keys['9'])
		{
			testing3 -= 0.1f;
			cout << testing1 << ":" << testing2 << ":" << testing3 << endl;
		}
		if (keys['0'])
		{
			testing3 += 0.1f;
			cout << testing1 << ":" << testing2 << ":" << testing3 << endl;
		}
		break;
	case 4:
		if (keys['9'])
		{
			pos[3] -= 10000;
			cout << pos[0] << ":" << pos[1] << ":" << pos[2] << ":" << pos[3] << endl;
		}
		if (keys['0'])
		{
			pos[3] += 10000;
			cout << pos[0] << ":" << pos[1] << ":" << pos[2] << ":" << pos[3] << endl;
		}
		break;
	case 5:
		if (keys['9'])
		{
			testing -= 1.0f;
			cout << "Value: " << testing << endl;
		}
		if (keys['0'])
		{
			testing += 1.0f;
			cout << "Value: " << testing << endl;
		}
		break;
	case 6:
		if (keys['9'])
		{
			testing -= 0.1f;
			cout << "Value: " << testing << endl;
		}
		if (keys['0'])
		{
			testing += 0.1f;
			cout << "Value: " << testing << endl;
		}
		break;
	}


	//cout << "SpeedX: " << objectXspeed << " SpeedY: " << objectYspeed << " SpeedZ: " << objectZspeed << endl;

	float mZDetect = mZ + objectZspeed;
	float mYDetect = mY + objectYspeed;
	float mXDetect = mX + objectZspeed;

	if (earthDetection(mXDetect, mYDetect, mZDetect))
	{
		if (detectState == 0)
		{
			diffX = mXDetect - oldmX;
			diffY = mYDetect - oldmY;
			diffZ = mZDetect - oldmZ;
			detectState = 1;
		}
		objectSpeed = 0;
		objectZspeed = 0;
		objectYspeed = 0;
		objectXspeed = 0;

		glm::vec3 cord = earth->getCoordinates();
		glm::vec3 cord2 = spaceship->getCoordinates();
		glm::vec3 cord3 = cord - cord2;
		mX -= cord3[0] / 10000;
		mY -= cord3[1] / 10000;
		mZ -= cord3[2] / 10000;
	}
	else
	{
		if (winterDetection(mXDetect, mYDetect, mZDetect))
		{
			if (detectState == 0)
			{
				diffX = mXDetect - oldmX;
				diffY = mYDetect - oldmY;
				diffZ = mZDetect - oldmZ;
				detectState = 1;
			}
			objectSpeed = 0;
			objectZspeed = 0;
			objectYspeed = 0;
			objectXspeed = 0;

			glm::vec3 cord = winter->getCoordinates();
			glm::vec3 cord2 = spaceship->getCoordinates();
			glm::vec3 cord3 = cord - cord2;
			mX -= cord3[0] / 10000;
			mY -= cord3[1] / 10000;
			mZ -= cord3[2] / 10000;
		}
		else
		{
			if (Ship_Material_Ambient[0] < 10)
			{
				oldmZ = mZ;
				oldmY = mY;
				oldmX = mX;
				mZ += objectZspeed * testing;
				mY += objectYspeed * testing;
				mX += objectXspeed * testing;
				detectState = 0;
			}
		}
	}

	spaceship->setSpeed(objectSpeed);

	if (keys['W'])
	{
		objectSpeed = objectSpeed - objectSpeed / 100;
	}
	else
	{
		objectSpeed = objectSpeed - objectSpeed / 10;
	}
	

	mouseXLast = mouse_x;
	mouseYLast = mouse_y;
	updateTransform(spinXinc, spinYinc, spinZinc);
	sunDetection();

}
void updateTransform(float xinc, float yinc, float zinc)
{
	glm::mat4 matrixX, matrixXY;

	//rotation about the local x axis
	q = glm::angleAxis(xinc, glm::vec3(objectRotation[0][0], objectRotation[0][1], objectRotation[0][2]));
	matrixX = glm::mat4_cast(q) * objectRotation;

	//EXAMPLE FOR ACCESSING USING A 1D array
	const float *pSource = (const float*)glm::value_ptr(matrixX);
	//rotation about the local y axis
	q = glm::angleAxis(yinc, glm::vec3(pSource[4], pSource[5], pSource[6]));
	matrixXY = glm::mat4_cast(q) * matrixX;

	//EXAMPLE ACCESSING WITH 2D GLM structure.
	//rotation about the local z axis
	q = glm::angleAxis(zinc, glm::vec3(matrixXY[2][0], matrixXY[2][1], matrixXY[2][2]));
	objectRotation = glm::mat4_cast(q) * matrixXY;

	cRotation = objectRotation;
}


bool earthDetection(float speedX, float speedY, float speedZ)
{
	glm::vec3 spShipCoord = spaceship->getCoordinates();
	glm::vec3 earthCoord = earth->getCoordinates();

	float distance = pow((speedX - earthCoord[0]), 2) + pow((speedY - earthCoord[1]), 2) + pow((speedZ - earthCoord[2]), 2);
	if (distance < pow(119050, 2)) { return true; }
	return false;
}
bool winterDetection(float speedX, float speedY, float speedZ)
{
	glm::vec3 spShipCoord = spaceship->getCoordinates();
	glm::vec3 winterCoord = winter->getCoordinates();

	float distance = pow((speedX - winterCoord[0]), 2) + pow((speedY - winterCoord[1]), 2) + pow((speedZ - winterCoord[2]), 2);
	if (distance < pow(73500, 2)) { return true; }
	return false;
}


void sunDetection()
{
	glm::vec3 spShipCoord = spaceship->getCoordinates();
	glm::vec3 sunCoord = sun->getCoordinates();
	float distance = pow((spShipCoord[0] - sunCoord[0]), 2) + pow((spShipCoord[1] - sunCoord[1]), 2) + pow((spShipCoord[2] - sunCoord[2]), 2);

	if (distance < pow(6000000, 2))
	{
		int multiplayer = 1;
		if (distance < pow(5000000, 2)) { multiplayer = 3; }
		if (distance < pow(4000000, 2)) { multiplayer = 9; }
		if (distance < pow(3000000, 2)) { multiplayer = 27; }
		if (distance < pow(2000000, 2)) { multiplayer = 81; }
		Ship_Material_Ambient[0] += 0.0001f + 2000000000 / distance * multiplayer;;
		cout << 2000000000 / distance << " " << Ship_Material_Ambient[0] << endl;
	}
	else
	{
		if (Ship_Material_Ambient[0] > 0)
		{
			Ship_Material_Ambient[0] -= 0.001f;
		}
	}
}


/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	AllocConsole();
	FILE *stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
	
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop

	// Create Our OpenGL Window
	if (!CreateGLWindow("OpenGL Win32 Example",screenWidth,screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(keys[VK_ESCAPE])
				done = true;
			keyboard();

			display();					// Draw The Scene
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = true;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}
		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		case WM_LBUTTONDOWN:
		{
	        mouse_x = LOWORD(lParam);          
			mouse_y = screenHeight - HIWORD(lParam);
			leftPressed = true;
		}
		break;

		case WM_LBUTTONUP:
		{
			leftPressed = false;
		}
		break;

		case WM_RBUTTONDOWN:
		{
			mouse_x = LOWORD(lParam);
			mouse_y = screenHeight - HIWORD(lParam);
			rightPressed = true;
		}
		break;

		case WM_RBUTTONUP:
		{
			rightPressed = false;
		}
		break;

		case WM_MOUSEMOVE:
		{
	        mouse_x = LOWORD(lParam);          
			mouse_y = screenHeight  - HIWORD(lParam);
		}
		break;

		case WM_MOUSEWHEEL:
		{
			zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			wheelMoved = true;
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	HGLRC tempContext = wglCreateContext(hDC);
	wglMakeCurrent(hDC, tempContext);
	
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << " GLEW ERROR" << endl;
		
	}
	
	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};
	
    if(wglewIsSupported("WGL_ARB_create_context") == 1)
    {
		hRC = wglCreateContextAttribsARB(hDC,0, attribs);
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(hDC, hRC);
	}
	else
	{	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		hRC = tempContext;
		cout << " not possible to make context "<< endl;
	}

	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	cout << "OpenGL version: " << GLVersionString << endl;

	//We can check the version in OpenGL 
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	cout << "OpenGL Version: " << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	
	init();

	reshape(width, height);     					// Set Up Our Perspective GL Screen
	
	
	return true;									// Success
}



