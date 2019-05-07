#pragma once

//These pragma commands can be used to ENSURE that the struct is the exact size it should be.
//I've yet to run into the issues that sizeof(>>InsertStruct<<) is inaccurate but according to one of my sources it could happen.
#include <vector>

#pragma pack(push)
#pragma pack(1)
struct MehHeader
{
	unsigned int meshCount;
	unsigned int materialCount;

};
#pragma pack(pop)

struct MeshGroup
{
	char groupName[256];
	float transformation[3];
	float rotation[3];
	bool isChild;
	char parent[256];
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

struct GeoTransformations
{
	float translation[3];
	float rotation[3];
	float scale[3];
};

struct MeshVert
{
	Vertex* vertices;
};

struct LoadedMesh
{
	char name[256];
	char materialName[256];
	//char childName;

	int type;
	int link;

	unsigned int vertexCount;
};

struct MeshHolder
{
	char name[256];
	char materialName[256];
	//vector<MeshHolder> children;

	// Might make this into a vector
	//vector<Vertex> vertices;
	unsigned int vertexCount;
	Vertex* vertices;

	int type;
	int link;

	// Constructor that may not be needed
	MeshHolder()
	{
		vertices = nullptr;
	}
};

struct Texture
{
	//What material this texture is connected to.
	int MatIndex;
	//What index in a materials array of textures this texture belongs to.
	int texID;
	unsigned char textureName[256];
};

struct LightSource
{
	float pos[3];
	float direction[3];
};


