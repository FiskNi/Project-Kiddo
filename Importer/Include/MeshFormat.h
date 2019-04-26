#pragma once

//Make sure that the size of the struct is exact.
#pragma pack(push)
#pragma pack(1)
struct mehHeader
{
	//unsigned int meshGroupAmount;
	/*unsigned int nrOfVerticies;*/
	unsigned int nrOfMeshes;
	//unsigned int animationAmount;
	//unsigned int lightSourceAmount;

	//char fileName[4];
};
#pragma pack(pop)

struct meshGroup
{
	char groupName[256];
	float transformation[16];
	bool isChild;
	char parentName[256];
};

struct vertex
{
	float pos[3];
	float uv[2];
	float normal[3];
	float tangent[3];
	float biNormal[3];
};

struct mesh
{
	char meshName[256];
	//unsigned int meshID;
	//float transformation[16];
	//bool isChild;
	//char parentName[256];
	unsigned int nrOfMaterials;
	unsigned int nrOfVerticies;
	//unsigned int bones;
	//bool customAttribute;
	//char customAttributeName[256];
};

struct material
{
	int index;
	char materialName[256];
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float emissive[3];
	float opacity;
	float shininess;
	float reflectivity;
	int nrOfTextures;
	//unsigned char albedoFile[256];
	//unsigned char normalFile[256];
};

#pragma pack(push)
#pragma pack(1)
struct texture
{
	//What material this texture is connected to.
	int MatIndex;
	//What index in a materials array of textures this texture belongs to.
	int texID;
	unsigned char textureName[256];
};
#pragma pack(pop)

struct lightSource
{
	float pos[3];
	float direction[3];
};


