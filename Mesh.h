#pragma once
#include "Headers.h"
//#include "MeshGroupClass.h"
//============================================================================
//	- Mesh
//	Handles pure mesh data.
//	This is where everything the renderer needs for the shader inputs and nothing else.
//	Currently the mesh also holds onto the material id. This could be changed to make
//	entities hold their own material id.
//============================================================================

#ifndef MESH_H
#define MESH_H

struct SkeletonD
{
	struct JointD
	{
		string name;
		int parentIndex;
		glm::mat4 invBindPose;
	};

	struct AnimationD
	{
		struct KeyFrameD
		{
			int id;
			// local transform, good for interpolation and then making a final global.
			vector<glm::vec3>	local_joints_T;
			vector<glm::quat>	local_joints_R;
			vector<glm::vec3>	local_joints_S;
		};
		string name;
		int keyframeFirst;
		int keyframeLast;
		float duration;
		float rate;
		vector<KeyFrameD> keyframes;
	};

	string name;
	vector<JointD> joints;
	vector<AnimationD> animations;
	float currentAnimTime;
	bool playingBackwards;
};

class MeshGroupClass;

class Mesh
{
private:
	glm::vec3 position;
	glm::vec3 rotationEulerXYZ;
	glm::quat rotation;
	glm::vec3 scale;

	int vertexCount;
	std::vector<vertexPolygon> vertices;

	string name;
	string pName;
	bool isChild;

	//-1 is No Parent, 0 is mesh, 1 is group
	int parentType;

	Mesh* myParent;
	MeshGroupClass* myGroupParent;
	SkeletonD skeleton;

	glm::vec3 parentPosOffset;
	glm::vec3 parentSizeOffset;

	unsigned int materialID;

public:
	Mesh(Vertex* vertArr, unsigned int vertexCount);
	Mesh(Loader* inLoader, int index);
	Mesh();
	~Mesh();

	void CreateCubeData();
	void CreatePlaneData();
	void CreatePlateData();
	void ImportMesh(Vertex* verticies, int vertexCount);

	void CalculateTangents();

	void setMaterial(unsigned int texID);

	void SetPosition(glm::vec3 newPos);
	void SetPosition(float x, float y, float z);
	void SetPositionX(float x);
	void SetPositionY(float y);
	void SetPositionZ(float z);

	void SetRotation(glm::quat newRot);

	void SetScale(glm::vec3 newSca);
	void SetScale(float x, float y, float z);

	void ForwardTime(float t);
	void BackwardTime(float t);
	void SetPlayingBackwards(bool tf);
	void SetTime(float t);

	std::vector<vertexPolygon>& ModifyVertices();

	unsigned int GetMaterialID() const { return materialID; }
	glm::vec3 GetPosition() const { return position; }
	glm::quat GetRotation() const { return rotation; }
	glm::vec3 GetScale() const { return scale; }
	string GetMeshName() const { return name; }
	string GetMeshParentName() const { return pName; }
	bool GetIsChild() const { return isChild; }
	int GetParentType() const { return parentType; }
	glm::vec3 GetParentPosOffset() const { return parentPosOffset; }
	glm::vec3 GetParentSizeOffset() const { return parentSizeOffset; }

	void SetMeshParent(Mesh *parent);
	void SetGroupParent(MeshGroupClass * parent);
	void SetParentPosOffset(glm::vec3 offset);
	void SetParentSizeOffset(glm::vec3 offset);

	Mesh* GetMeshParent() { return myParent; }
	MeshGroupClass* GetGroupParent() { return myGroupParent; }

	std::vector<vertexPolygon> GetVertices() { return vertices; }
	SkeletonD& GetSkeleton() { return skeleton; }
	int GetVertexCount() const { return vertexCount; }
	
	//bool GetPLayingBackwards() { return skeleton.playingBackwards; }
};

#endif MESH_H


