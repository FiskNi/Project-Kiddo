#pragma once
#include "Headers.h"

class ShaderHandler
{
public:
	ShaderHandler();
	~ShaderHandler();

	void CreateShader(const char* vertexShader, const char* fragmentShader);
	void CreateFSShaders();

	void CreateFullScreenQuad();

	// Deprecated | The VBO is now created and tied to a VAO where the vertrices are specified
	void createVertexBuffer(std::vector<vertexPolygon> vertices);

	GLuint getVertexAttributes();
	GLuint getShader();


private:
	GLuint gShaderProgram;

	// To be deleted
	GLuint gVertexBuffer;
	GLuint gVertexAttribute;

};

