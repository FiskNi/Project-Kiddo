#pragma once
#include "Headers.h"

class ShaderHandler
{
public:
	ShaderHandler();
	~ShaderHandler();

	void CreateShaders(GLuint* gShaderProgram);
	void CreateFSShaders(GLuint *gShaderProgram);

private:
	//GLuint gShaderProgram;
};

