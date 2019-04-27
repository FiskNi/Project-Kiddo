#pragma once
#include "Headers.h"

//============================================================================
//	Simply compiles a shader from the input files
//============================================================================
class Shader
{
private:
	GLuint gShaderProgram;

	// To be deleted
	GLuint gVertexBuffer;
	GLuint gVertexAttribute;

public:
	Shader();
	~Shader();

	// Could be moved to constuctor
	void CreateShader(const char* vertexShader, const char* fragmentShader);

	// Fullscreen quad stuff. Can be moved into its own class whenever.
	void CreateFSShaders();
	void CreateFullScreenQuad();

	GLuint getVertexAttributes() { return gVertexAttribute; }
	GLuint getShader() { return gShaderProgram; }

};

