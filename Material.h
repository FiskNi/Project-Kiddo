#pragma once
#include "Headers.h"

//============================================================================
//	- Materials
//	A material will hold specific data that can be shared between multiple objects.
//	For each object to know what material it will use during rendering a 
//	material ID needs to be assigned.
//
//	- Usage:
//	The material ID is constructed (currently) manually on material creation
//	and also manually applied to the object that will use this material.
//		(See the constuctor)
//	The ID can be any number as long as it doesn't conflict with an existing material*
//	and is linked correctly to the object using it.
//																*In a scene
//
//	You should never need to "get" a specific texture and should instead use 
//	the material ID when assigning materials. Exceptions are if creating specific shaders.
//============================================================================

class Material
{
private:
	char* name;
	unsigned int materialID;
	GLuint albedo;
	GLuint normal;

	bool hasNormalmap;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emissive;
	float opacity;


public:
	Material(char* name, unsigned int id);
	Material(PhongMaterial material, unsigned int id);
	~Material();

	void createAlbedo(std::string path);
	void createNormal(std::string path);

	unsigned int getMaterialID() const { return materialID; }

	// For the renderer only
	GLuint getAlbedo() const { return albedo; }
	GLuint getNormal() const { return normal; }
	bool hasNormal() const { return hasNormalmap; }

	glm::vec3 getAmbient() const { return ambient; }
	glm::vec3 getDiffuse() const { return diffuse; }
	glm::vec3 getSpecular() const { return specular; }
	glm::vec3 getEmissive() const { return emissive; }

};



