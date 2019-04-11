#pragma once
#include "Headers.h"
class Material
{
private:
public:
	Material();
	~Material();

	GLuint createTexture(std::string path);
};

