#pragma once
#include "Headers.h"
class Material
{
private:
	GLuint texture;
public:
	Material();
	~Material();

	void createTexture(std::string path);

	GLuint getTexture() const;
};

