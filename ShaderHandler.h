#pragma once
#include "Headers.h"

class ShaderHandler
{
public:
	ShaderHandler();
	~ShaderHandler();

	void CreateShaders(const char* vertexShader, const char* fragmentShader);
	void CreateFSShaders();

	void CreateFullScreenQuad();

	void createVertexBuffer(std::vector<vertexPolygon> vertices);

	GLuint getVertexAttributes();


	GLuint getShader();


private:
	GLuint gShaderProgram;

	GLuint gVertexBuffer;
	GLuint gVertexAttribute;

};

