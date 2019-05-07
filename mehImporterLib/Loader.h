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
	MeshGroup* meshGroup;
	PhongMaterial* material;
	MeshVert* meshVert;
	LoaderMesh* mesh;

public:
	Loader(std::string fileName);
	~Loader();

	int GetHeader() { return fileHeader.meshCount; }
	int GetMeshCount() { return fileHeader.meshCount; }
	std::string GetFileName() { return fileName; }

	Vertex *GetVerticies(int meshID) { return meshVert[meshID].vertices; }
	int GetVertexCount(int meshID) { return mesh[meshID].vertexCount; }

	// New, remove above when implemented
	LoaderMesh GetMesh(int index) const { return mesh[index]; }
	int GetType(int index) const { return mesh[index].type; }
	int GetLink(int index) const { return mesh[index].link; }
	
	PhongMaterial GetMaterial(int index) const { return material[index]; }
	int GetMaterialCount() const { return fileHeader.materialCount; }
};
