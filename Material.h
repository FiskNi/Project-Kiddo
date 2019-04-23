#pragma once
#include "Headers.h"

//============================================================================
//	- Materials
//	A material will hold specific data that can be shared between multiple objects.
//	For each object to know what material it will use during rendering 
//	a material ID needs to be assigned.
//
//	- Usage:
//	This material ID is constructed (currently) manually on material creation
//	and also manually applied to the object that will use this material.
//		(See the constuctor)
//	The ID can be any number as long as it doesn't conflict with an existing material*
//	and is linked correctly to the object using it.
//																*In a scene
//
//	You should never need to "get" a specific texture and should instead use the material ID for this
//============================================================================

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



