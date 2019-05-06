#pragma once
#include <iostream>
#include <string.h>
#include <fstream>
#include "MeshFormat.h"

class Loader 
{
private:
	std::string fileName;
	MehHeader * meshHeader;
	MeshGroup * meshGroup;
	PhongMaterial * material;
	GeoTransformations * geoTransform;
	MeshVert * meshVert;
	LoadedMesh * mesh;
	MeshHolder * meshHolder;
	//material * materialArr;
	//material ** materialArr;

	unsigned int meshCount;
	unsigned int materialCount;

	Texture *** textures;

public:
	Loader(std::string fileName);
	~Loader();

	int getHeader();
	int getNrOfMeshes();
	std::string getFileName();

	Vertex *getVerticies(int meshID);
	int getNrOfVerticies(int meshID);

	char* getAlbedo()const;
	char* getNormal()const;

	//Stubs. Yet unimplemented. Where meshID is the meshes position in the array.
	LoadedMesh getMesh(int meshID);
	PhongMaterial getMaterial(int meshID, int materialID);
	PhongMaterial * getAllMaterials(int meshID);
	Texture getTexture(int meshID, int materialID, int textureID);
	Texture ** getAllTextures(int meshID);

};
