#pragma once
#include "Headers.h"
class Material
{
private:
	char* name;
	unsigned int materialID;
	GLuint albedo;
	GLuint normal;

	bool hasNormalmap;

public:
	Material(char* name, unsigned int id);
	~Material();

	void createAlbedo(std::string path);
	void createNormal(std::string path);

	unsigned int getMaterialID() const;
	GLuint getAlbedo() const;
	GLuint getNormal() const;
	bool hasNormal() const;
};

