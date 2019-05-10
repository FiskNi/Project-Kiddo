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
	DirLight* dirLight;
	PointLight* pointLight;
	

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
	
	float getDirLightPos(int index, int posIndex) const { return dirLight[index].position[posIndex]; }
	float getDirLightIntensity(int index) const { return dirLight[index].intensity; }
	float getDirLightRotation(int index, int rotIndex) const { return dirLight[index].rotation[rotIndex]; }
	float getDirLightColor(int index, int colIndex) const { return dirLight[index].color[colIndex]; }
	int getDirLightCount() const { return fileHeader.dirLightCount; }

	float getPointLightPos(int index, int posIndex) const { return pointLight[index].position[posIndex]; }
	float getPointLightIntensity(int index) const { return pointLight[index].intensity; }
	int getPointLightCount() const { return fileHeader.pointLightCount; }

	PhongMaterial GetMaterial(int index) const { return material[index]; }
	int GetMaterialCount() const { return fileHeader.materialCount; }
};
