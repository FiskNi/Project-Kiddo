#pragma once
#include <iostream>
#include <string.h>
#include <fstream>
#include "MeshFormat.h"

class Loader 
{
private:
	std::string fileName;
	mesh * meshArr;
	material ** materialArr;

	texture *** textures;

	unsigned int meshCount;
	unsigned int materialCount;

public:
	Loader(std::string fileName);
	~Loader();

	int getHeader();
	int getNrOfMeshes();
	std::string getFileName();

	vertex *getVerticies(int meshID);
	int getNrOfVerticies(int meshID);

	//Stubs. Yet unimplemented. Where meshID is the meshes position in the array.
	mesh getMesh(int meshID);
	material getMaterial(int meshID, int materialID);
	material * getAllMaterials(int meshID);
	texture getTexture(int meshID, int materialID, int textureID);
	texture ** getAllTextures(int meshID);

};
