#pragma once

#ifndef CUBE_H
#define CUBE_H

#include "..\gl\glew.h"

class Shader;

const int NumberOfVertexCoords = 24;
const int NumberOfTriangleIndices = 36;

class Cube
{
private:
	float dim;
	unsigned int m_vaoID;		    // vertex array object
	unsigned int m_vboID[2];		// two VBOs - used for colours and vertex data
	GLuint ibo;                     //identifier for the triangle indices

	static int numOfVerts;
	static int numOfTris;

	float verts[NumberOfVertexCoords];
	float cols[NumberOfVertexCoords];
	unsigned int tris[NumberOfTriangleIndices];

	float x;
	float y;
	float z;



public:
	Cube();
	void setDim(float d);
	void constructGeometry(Shader* myShader);
	void render();
	float getDim();
	void setCoordinates(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	float getX()
	{
		return x;
	}
	float getY()
	{
		return y;
	}
	float getZ()
	{
		return z;
	}
};

#endif