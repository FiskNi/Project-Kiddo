#pragma once
#include "Headers.h"

class SimpleShape
{
public:
	SimpleShape();
	~SimpleShape();
	void CreateTriangleData(GLuint gShaderProgram, GLuint gVertexAttribute, GLuint gVertexBuffer);
};

