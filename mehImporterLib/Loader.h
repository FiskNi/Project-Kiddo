#pragma once
#include <iostream>
#include <string.h>
#include <fstream>
#include "MeshFormat.h"

class Loader 
{
private:
	std::string fileName;
	MehHeader fileHeader;
	MeshGroup * meshGroup;
	PhongMaterial * material;
	MeshVert * meshVert;
	LoadedMesh * mesh;
	MeshHolder * meshHolder;
	//material * materialArr;
	//material ** materialArr;

	int meshCount;
	int groupCount;
	int materialCount;
	int pointLightCount;
	int dirLightCount;


public:
	Loader(std::string fileName);
	~Loader();

	int getHeader();
	int getNrOfMeshes();
	std::string getFileName();

	Vertex *getVerticies(int meshID);
	int getNrOfVerticies(int meshID);

	char* getAlbedo(int materialID)const;
	char* getNormal(int materialID)const;
	MehHeader GetMeshData(int index) const;


	//Stubs. Yet unimplemented. Where meshID is the meshes position in the array.
	LoadedMesh getMesh(int meshID);
	PhongMaterial getMaterial(int meshID, int materialID);
	PhongMaterial * getAllMaterials(int meshID);
};
