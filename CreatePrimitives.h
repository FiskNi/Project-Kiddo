#pragma once
#include "Headers.h"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class CreatePrimitive
{
private:
	GLuint gVertexBuffer;
	GLuint gVertexAttribute;


public:
	CreatePrimitive();
	~CreatePrimitive();

	void CreateTriangleData(GLuint shaderID, float test);

	GLuint getVertexAttribute() const;
};

