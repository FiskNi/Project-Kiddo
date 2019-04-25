#pragma once
#include <iostream>
#include <fstream>
#include "MeshFormat.h"

class Loader 
{
private:
	std::string fileName;
	mehHeader header;
	mesh * meshArr;
	vertex ** dataArr;
	material ** materials;


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

};
