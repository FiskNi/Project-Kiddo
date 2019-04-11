#pragma once
#include "Headers.h"

class ShaderHandler
{
public:
	ShaderHandler();
	~ShaderHandler();

	void CreateShaders(GLuint* gShaderProgram, const char* vertexShader, const char* fragmentShader);
	void CreateShaders(GLuint* gShaderProgram, const char* vertexShader,const char* geometryShader, const char* fragmentShader);
	void CreateFSShaders(GLuint *gShaderProgram);

	GLint getShader(int indice);

private:


};

