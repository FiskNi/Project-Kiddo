#pragma once

//These pragma commands can be used to ENSURE that the struct is the exact size it should be.
//I've yet to run into the issues that sizeof(>>InsertStruct<<) is inaccurate but according to one of my sources it could happen.
#include <vector>

struct MehHeader
{
	int meshCount;
	int groupCount;
	int materialCount;
	int pointLightCount;
	int dirLightCount;
};

struct MeshGroup
{
	char groupName[256];
	
	float translation[3];
	float rotation[3];
	float scale[3];

	bool isChild;
	char parentName[256];
	int parentType;
};

struct Vertex
{
	float position[3];
	float uv[2];
	float normal[3];
	float tangent[3];
	float bitangent[3];
};

struct PhongMaterial
{
	char name[256];
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float emissive[3];
	float opacity;

	char albedo[256];
	char normal[256];
};

struct MeshVert
{
	Vertex* vertices;
};

struct LoaderMesh
{
	char name[256];
	unsigned int materialID;

	float translation[3];
	float rotation[3];
	float scale[3];

	bool isChild;
	char parentName[256];
	int parentType;

	int type;
	int link;
	int dir;
	float dist;

	int vertexCount;
};

struct DirLight
{
	float position[3];
	float rotation[3];
	float color[3];
	float intensity;
};

struct PointLight 
{
	float position[3];
	float color[3];
	float intensity;
};