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
	LoaderMesh* mesh;
	DirLight* dirLight;
	PointLight* pointLight;

	MeshVert* meshVert;
	std::vector<MeshAnis> animationsD;
	std::vector<MeshSkeleton> skeletonsD;

public:
	Loader(std::string fileName);
	~Loader();

	int GetHeader()				{ return fileHeader.meshCount; }
	int GetMeshCount()			{ return fileHeader.meshCount; }
	std::string GetFileName()	{ return fileName; }

	Vertex *GetVerticies(int meshID)	{ return meshVert[meshID].vertices; }
	int GetVertexCount(int meshID)		{ return mesh[meshID].vertexCount; }

	// New, remove above when implemented
	LoaderMesh GetMesh(int index) const												{ return mesh[index]; }
	Skeleton GetSkeleton(int index) const											{ return mesh[index].skeleton; }
	Joint GetJoint(int mIndex, int jIndex) const									{ return skeletonsD[mIndex].joint[jIndex]; }
	Animation GetAnimation(int mIndex, int aIndex) const							{ return animationsD[mIndex].animations[aIndex].ani; }
	KeyFrame GetKeyFrame(int mIndex, int aIndex, int kIndex) const					{ return animationsD[mIndex].animations[aIndex].keyFrames[kIndex].key; }
	Transform GetTransform(int mIndex, int aIndex, int kIndex, int tIndex) const	{ return animationsD[mIndex].animations[aIndex].keyFrames[kIndex].transforms[tIndex].t; }



	int GetType(int index) const { return mesh[index].type; }
	int GetLink(int index) const { return mesh[index].link; }
	int GetCollectIndex(int index) const { return mesh[index].collect; }
	
	float* GetDirLightPos(int index) const		{ return dirLight[index].position; }
	float GetDirLightIntensity(int index) const { return dirLight[index].intensity; }
	float* GetDirLightRotation(int index) const { return dirLight[index].rotation; }
	float* GetDirLightColor(int index) const	{ return dirLight[index].color; }
	int GetDirLightCount() const				{ return fileHeader.dirLightCount; }

	//float GetPointLightPos(int index, int posIndex) const { return pointLight[index].position[posIndex]; }
	float* GetPointLightPos(int index) const	{ return pointLight[index].position; }
	float* GetPointLightColor(int index) const	{ return pointLight[index].color; }

	float GetPointLightIntensity(int index) const	{ return pointLight[index].intensity; }
	int GetPointLightCount() const					{ return fileHeader.pointLightCount; }

	unsigned int GetMaterialID(int meshIndex) const { return mesh[meshIndex].materialID; }
	PhongMaterial GetMaterial(int index) const		{ return material[index]; }
	int GetMaterialCount() const					{ return fileHeader.materialCount; }

	int GetMeshGroupCount() const					{ return fileHeader.groupCount; }
	MeshGroup GetMeshGroup(int index) const			{ return meshGroup[index]; }
	float* GetParentsOffset(char* parentName);
};
