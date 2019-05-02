#pragma once

//These pragma commands can be used to ENSURE that the struct is the exact size it should be.
//I've yet to run into the issues that sizeof(>>InsertStruct<<) is inaccurate but according to one of my sources it could happen.
#pragma pack(push)
#pragma pack(1)
struct mehHeader
{
	//unsigned int meshGroupAmount;
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

struct material
{
	unsigned int materialIndex;
	char materialName[256];
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float emissive[3];
	//float opacity;
	//float shininess;
	//float reflectivity;
	unsigned int nrOfTextures;
	//unsigned char albedoFile[256];
	//unsigned char normalFile[256];
};

struct mesh
{
	char meshName[256];
	char materialName[256];
	//unsigned int meshID;
	//float transformation[16];
	//bool isChild;
	//char parentName[256];
	//unsigned int materialIndex;
	unsigned int nrOfMaterials;
	unsigned int nrOfVerticies;
	vertex* vertices;
	//unsigned int bones;
	//bool customAttribute;
	//char customAttributeName[256];
};


struct texture
{
	//What material this texture is connected to.
	int MatIndex;
	//What index in a materials array of textures this texture belongs to.
	int texID;
	unsigned char textureName[256];
};

struct lightSource
{
	float pos[3];
	float direction[3];
};


